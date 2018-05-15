/*
reference: http://labdegaragem.com/forum/topics/ler-carga-da-bateria-que-alimenta-arduino

     Battery Characterization Tool

     11/7/2009 John Terry

     Compiled on Aurdino 1.52 in 5500 bytes.

 This uses the analog in to measure the voltage of a battery under a known load and
 integrates the area under the curve to arrive at the useful capacity of the
 batterin in mAH.

*/

#include <LiquidCrystal.h>


// Set some constants -- these will need to be adjusted for your setup
//
// Connect a ~10KΩ (Rr) resistor from 3.3V supply pin to the AREF pin,
//   creating a voltage divder with the internal 32KΩ resistor on AREF.
//      aRefVoltage = supply_voltage * 32K / Rr
//   Alternatively, measure the actual AREF voltage applied with a good DMM:
//  my measured aRefVoltage = 2.62;
const float     aRefVoltage = 2.62;

// Connect a load resistor (Rl) to the battery. ~2.2Ω restistor gives ~500mA drain which
//   is about right for a battery rated at 2500mAH.  Note, this should be a >=1W resistor!
//   Dont trust your DMM to meausre such a low resistance accurately. I measured the current
//   and back-calculated the resistance. Best just to trust rated resistance.
//   my  resistance = 2.2;
//
// The integrator works by accumulating the sampled voltage values from the start until
//   hitting a pre-determined low-voltage threshold. Since they are sampled at a
//   known period, the number of samples taken cancels out and the accumulation of all samples
//   simply needs to be scalled by a factor
//
//   Let's make some definitions to show the derivation of how this is so:
//   I = current
//   V = load voltage
//   Rl = load resistance
//   samples = number of voltage samples made
//   sensor = value read out of analog A->D
//   rate = number of samples taken per second
//
//                                        ave(V)
//   capacity = ∫I ~= ave(I) * time = ----------- * time
//                                          Rl
//
//   Where:
//                ∑(V)     ∑(sensor) * aRefVoltage/1024
//     ave(V) = -------  = ---------------------------------
//              samples              samples
//
//      time  =  samples/rate
//
//   Thus:
//               ∑(sensor) * aRefVoltage        samples
//   capacity = ----------------------------- * ----------
//                samples * 1024 * Rl            rate
//
//               ∑(sensor) * aRefVoltage
//   capacity = ------------------------------ = ∑(sensor) * quanta
//                  1024 * Rl * rate
//
//
//                aRefVoltage
//   quanta =  --------------------
//               1024 * Rl * rate
//
//   rate in this sketch is 1 Hz (1/sec).
//   Thus, the units of quanta are: volt*seconds/(Ω*samples) = amp*seconds per sample
//
//   What we really want are mA*Hours, so, scale by 1hr/3600seconds and 1000mA/A, or simply 1/3.6
//
//   So, finally:
//   quanta = aRefVoltage/(Rl*1024*3.6)  in units of mA*hours per sample

const double quanta = 0.00032305; // 2.62/(2.2*1024*3.6)


// Define low voltage threshold where any remaining capacity is "unusable"
//        lowThreshold = 0.9V * 1024/aRefVoltage
const int lowThreshold = 354;

int sensorPin          = 0;    // select the analog input pin for the voltage measurement
int ledPin             = 13;   // select the pin for the LED
int fetGatePin         = 8;    // select the pin for the LED
int sensorValue        = 0;    // unscaled sensor output
float voltage          = 0;    // measured voltage
double mAH             = 0;    // Calculated current
long accumulator       = 0;    // sum of all unscalled sensor values sampled
int epoch              = 0;    // seconds since battery was inserted
int lowVolts           = 0;    // debounce the low voltage threshold
boolean done           = false;// Voltage has dropped below threshold
boolean batteryIn      = false;// Battery present

// initialize the the LCD library with the numbers of the interface pins
//  LCD Pins  --  RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11,  5,  4,  3,  2);


void setup() {
  // declare the ledPin and fetGatePin as an OUTPUT:
  pinMode(ledPin,     OUTPUT);
  pinMode(fetGatePin, OUTPUT);

  // set analog reference to external
  analogReference(EXTERNAL);

  // set up the LCD's number of rows and columns:
  lcd.begin(16, 2);

  // Print a helpful start-up message to the LCD.
  lcd.print("Insert battery");

  // DEBUG initialize serial communications at 9600 bps:
  // Serial.begin(9600);
}


void loop() {

  sensorValue = analogRead(sensorPin);


  if (!batteryIn && (sensorValue > 100)) {
    // Initialize upon the insertion of a "fresh" battery
    batteryIn   = true;
    done        = false;
    epoch       = 0;
    accumulator = 0;
    lowVolts    = 0;
    digitalWrite(ledPin, LOW);
    digitalWrite(fetGatePin, HIGH);
    // clear out when LCD when starting over
    lcd.setCursor(0, 0);
    lcd.print("vlts  mAH  Time ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  else if (batteryIn && done && (sensorValue <= 20)) {
    // consider the battery removed only after finishing the last measurement to
    // debounce a glitchy concact
    batteryIn=false;
  }
  else if (batteryIn) {
    // Running state during discharge state

    voltage = sensorValue*aRefVoltage/1024.0;


    // print voltage
    lcd.setCursor(0, 1);
    lcd.print(voltage);

    if (!done) {
      accumulator += sensorValue;
      // print mAH
      mAH = accumulator*quanta;
      if      (mAH <   10) { lcd.setCursor(8, 1); }  // adjust to make it perdy
      else if (mAH <  100) { lcd.setCursor(7, 1); }
      else if (mAH < 1000) { lcd.setCursor(6, 1); }
      else                 { lcd.setCursor(5, 1); }
      lcd.print(int(mAH));

    }

    // print current time
    lcd.setCursor(11, 1);
    lcd.print(epoch/60.0);
    lcd.setCursor(15, 1);
    lcd.print("m");

    if (!done) {

      // lowVolts requires 10 seconds in the last 20 before being done
      if (sensorValue < lowThreshold) { lowVolts++;}
      else if (lowVolts > 0)          { lowVolts--;}

      // If it's below threshold for 10 of 20 samples, bail out
      if (lowVolts > 10) {
        done=true;
        digitalWrite(fetGatePin, LOW);
        lcd.setCursor(0, 0);
        lcd.print("    mAH in     ");
        lcd.setCursor(1, 0);
        lcd.print(int(mAH));

        // put the time, in minutes in the upper right
        lcd.setCursor(11, 0);
        lcd.print(epoch/60.0);
        lcd.setCursor(15, 0);
        lcd.print("m");

        // Clear out the bottom line
        lcd.setCursor(4, 1);
        lcd.print("v      ");
      }
    }

    epoch++;
  } // batteryIn -- main routine



  if (done) {
    // When done, flash the LED to get attention
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(499);
  } else {
    // Since the processing takes some time prior to the delay, we'll assume 1mS
    // This could stand to be improved with an interrupt routine that is kicked off
    // before all the processing starts for each loop
    delay(999);
  }

/* DEBUG
  Serial.print("\nsensor = " );
  Serial.print(sensorValue);
  Serial.print("\t lowvolrs = " );
  Serial.print(lowVolts);
*/
}
