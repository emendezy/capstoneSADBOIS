# LED strip Handler

"""
Command to run lights example:

sudo PYTHONPATH="/home/pi/Desktop/rpi_ws281x/python/.:build/lib.linux-armv7l-2.7" python /home/pi/Desktop/rpi_ws281x/python/examples/strandtest.py -c

Command to run this led handler:

sudo PYTHONPATH="/home/pi/Desktop/rpi_ws281x/python/.:build/lib.linux-armv7l-2.7" python /home/pi/Desktop/capstoneSADBOIS/LEDs/ LEDHandler.py -c -spell "<spellType>" -lights <# of lights / 6>

"""
import time
from neopixel import *
import argparse

# LED strip configuration:
LED_COUNT         = 6      # num of LED pixels
LEDS_PER_SECTION  = 6       # num of leds in each cooldown strip
LED_PIN           = 18      # GPIO pin connected to the pixels (18 uses PWM!).
LED_FREQ_HZ       = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA           = 10      # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS    = 200     # Set to 0 for darkest and 255 for brightest
LED_INVERT        = False   # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL       = 0       # set to '1' for GPIOs 13, 19, 41, 45 or 53

# LED index bounds for different cooldowns
HEALING_LED_START    = 0
BURNING_LED_START    = 6
CRACKLING_LED_START  = 12
BASTION_LED_START    = 18
SECOND_LED_START     = 24

#######################################
# Define some LED fns

def colorWipe(strip, color, wait_ms=50):
    """Wipe color across display a pixel at a time."""
    for i in range(strip.numPixels()):
        strip.setPixelColor(i, color)
        strip.show()
        time.sleep(wait_ms/1000.0)

def updateCooldown(strip, start, color, numLit, wait_ms=50):
	end = start + numLit
	colorWipe(strip, Color(0,0,0), 10)
	for i in range(start, end):
		strip.setPixelColor(i, color)
		strip.show()
		# time.sleep(wait_ms/1000.0)

if __name__ == "__main__":
	# Process arguments
	parser = argparse.ArgumentParser()

	parser.add_argument('-spell', dest='spellType', required=True,
		help='this is the spellType string')
	parser.add_argument('-lights', dest='numberOfLights', required=True,
		type=int, help='this represents the number of lights on out of 6')
	args = parser.parse_args()

	# Create NeoPixel object with appropriate configuration.
	strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL)
	# Intialize the library (must be called once before other functions).
	strip.begin()

	print(args.spellType, args.numberOfLights)

	if args.spellType == 'healing_ward':
		updateCooldown(strip, HEALING_LED_START, Color(255, 102, 204), args.numberOfLights)

	if args.spellType == 'burning_brand':
		updateCooldown(strip, BURNING_LED_START, Color(255, 0, 0), args.numberOfLights)

	if args.spellType == 'crackling_bolt':
		updateCooldown(strip, CRACKLING_LED_START, Color(255, 255, 0), args.numberOfLights)

	if args.spellType == 'bastion':
		updateCooldown(strip, BASTION_LED_START, Color(179, 89, 0), args.numberOfLights)

	if args.spellType == 'second_wind':
		updateCooldown(strip, SECOND_LED_START, Color(255, 255, 255), args.numberOfLights)

	# startTime = time.clock()
	# currTime = time.clock()
	# while(currTime - startTime < startTime + 3.0):
	# 	currTime = time.clock()

	# colorWipe(strip, Color(0,0,0), 10)
