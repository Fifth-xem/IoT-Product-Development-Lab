
from gpiozero import AngularServo
from time import sleep

# Create the servo object
servo = AngularServo(
    18,
    min_angle=0,
    max_angle=180,
    min_pulse_width=0.001,
    max_pulse_width=0.002
)

try:
    while True:
        print("Servo at 0 degrees")
        servo.angle = 0
        sleep(2)

        print("Servo at 90 degrees")
        servo.angle = 90
        sleep(2)

        print("Servo at 180 degrees")
        servo.angle = 180
        sleep(2)

except KeyboardInterrupt:
    print("Program stopped")

finally:
    servo.close()
