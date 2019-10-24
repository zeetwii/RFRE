import numpy as np
from textwrap import wrap
import math

#Main method for program
def main():
    print("\nADS-B Test program\n")

    print("Press 1 to test everything")
    print("Press 2 to test aircraft identification")
    print("Press 3 to test aircraft location")
    print("Press 4 to test aircraft crc")

    #User input for mode
    selection = input("Enter your choice: ")

    if selection == "1":
        print("\ndoing everything")

        icao = input("Enter ICAO address (hex):\n")
        addressMsg = aircraftID(icao)

        alt = int(input("Enter altitude in feet: "))
        lat = float(input("Enter lat in decimal format: "))
        lon = float(input("Enter lon in decimal format: "))

        evenLocMsg = position(lat, lon, alt, False)
        oddLocMsg = position(lat, lon, alt, True)

        evenMsg = addressMsg + evenLocMsg
        oddMsg = addressMsg + oddLocMsg

        #evenCRC = crc(format(int(evenMsg, 2), 'x'))
        #oddCRC = crc(format(int(oddMsg, 2), 'x'))
        #evenMsg = evenMsg + format(int(evenCRC, 16), 'b')
        #oddMsg = oddMsg + format(int(oddCRC, 16), 'b')


        evenCRC = crc(evenMsg)
        oddCRC = crc(oddMsg)
        evenMsg = evenMsg + evenCRC
        oddMsg = oddMsg + oddCRC


        print("\nEven: ")
        print("Bin: " + evenMsg)
        print("hex: " + bin2hex(evenMsg))
        print("encoded: " + encodeMsg(evenMsg))

        print("\nOdd: ")
        print("Bin: " + oddMsg)
        print("hex: " + bin2hex(oddMsg))
        print("encoded: " + encodeMsg(oddMsg))



    elif selection == "2":
        print("\ndoing ID")
        icao = input("Enter ICAO address (hex):\n")
        adsbMsg = aircraftID(icao)

        print ("ID: " + adsbMsg)
        print ("hex: " + format(int(adsbMsg, 2), 'x'))
    elif selection == "3":
        print("\ndoing location")

        alt = int(input("Enter altitude in feet: "))
        lat = float(input("Enter lat in decimal format: "))
        lon = float(input("Enter lon in decimal format: "))

        print("Even: " + format(int(position(lat, lon, alt, False), 2), 'x'))
        print(" Odd: " + format(int(position(lat, lon, alt, True), 2), 'x'))

    else:
        print("\n testing CRC")
        adsbMsg = input("Enter message in hex withoout CRC: \n")
        
        adsbMsg = adsbMsg + crc(adsbMsg)
        
        print(adsbMsg)
        print(format(int(adsbMsg, 2), 'x'))


#method to generate aircraft ID fields (DF, CA, ICAO)
def aircraftID(icao):
    """
    generates DF, CA, and ICAO fields of ADS-B message
    DF (downlink format) Field: 5 Bits
    CA (Capability identifiers): 3 bits
    ICAO address: 24 bits
    
    """

    #the message that we will be returning
    msg = ''

    #DF 17 is ADS-B
    df = format(17, 'b')
    
    #CA is 5 for to match type code later
    ca = format(5, 'b')
    
    #The full message
    msg = msg + df + ca + format(int(icao,16),'b')

    while (len(msg) < 32):
        msg = "0" + msg

    return msg

def hex2bin(hexstr):
    """Convert a hexdecimal string to binary string, with zero fillings."""
    num_of_bits = len(hexstr) * 4
    binstr = bin(int(hexstr, 16))[2:].zfill(int(num_of_bits))
    return binstr


def hex2int(hexstr):
    """Convert a hexdecimal string to integer."""
    return int(hexstr, 16)


def int2hex(n):
    """Convert a integer to hexadecimal string."""
    # strip 'L' for python 2
    return hex(n)[2:].rjust(6, "0").upper().rstrip("L")


def bin2int(binstr):
    """Convert a binary string to integer."""
    return int(binstr, 2)


def bin2hex(hexstr):
    """Convert a hexdecimal string to integer."""
    return int2hex(bin2int(hexstr))


def bin2np(binstr):
    """Convert a binary string to numpy array."""
    return np.array([int(i) for i in binstr])


def np2bin(npbin):
    """Convert a binary numpy array to string."""
    return np.array2string(npbin, separator="")[1:-1]

def adsbMod(x, y):
    """ADS-B Mod method"""
    return float(x - (y * math.floor(x/y)))


#CRC method
def crc(msg):
    """
    Mode-S Cyclic Redundancy Check.
    Detect if bit error occurs in the Mode-S message. When encode option is on,
    the checksum is generated.
    Args:
        msg (string): 22 bytes hexadecimal message string
    Returns:
        int: message checksum, or partity bits (encoder)
    """
    # the CRC generator
    G = [int("11111111", 2), int("11111010", 2), int("00000100", 2), int("10000000", 2)]

    #adding crc bytes to messgae
    #msg = msg + "000000"
    msg = msg + "000000000000000000000000"

    #msgbin = hex2bin(msg)
    #msgbin_split = wrap(msgbin, 8)
    msgbin_split = wrap(msg, 8)
    mbytes = list(map(bin2int, msgbin_split))

    for ibyte in range(len(mbytes) - 3):
        for ibit in range(8):
            mask = 0x80 >> ibit
            bits = mbytes[ibyte] & mask

            if bits > 0:
                mbytes[ibyte] = mbytes[ibyte] ^ (G[0] >> ibit)
                mbytes[ibyte + 1] = mbytes[ibyte + 1] ^ (
                    0xFF & ((G[0] << 8 - ibit) | (G[1] >> ibit))
                )
                mbytes[ibyte + 2] = mbytes[ibyte + 2] ^ (
                    0xFF & ((G[1] << 8 - ibit) | (G[2] >> ibit))
                )
                mbytes[ibyte + 3] = mbytes[ibyte + 3] ^ (
                    0xFF & ((G[2] << 8 - ibit) | (G[3] >> ibit))
                )

    result = (mbytes[-3] << 16) | (mbytes[-2] << 8) | mbytes[-1]

    result = str(format(result, 'b'))

    while (len(result) < 24):
        result =  "0" + result

    #print(result)

    return result
    #eturn format(int(result, 2), 'x')


#Position method (Lat, Lon, Alt)
def position(lat, lon, alt, odd):
    """
    Mode S extended positon generator.  
    Takes in lat, lon, and alt as ints
    Takes in odd as a boolean for even/odd frame

    Type code hard coded to 0x58

    returns a hex string
    """

    #perform altitude and TC math
    altitude = (alt + 1000)/25

    #explicitly casting as int because python can be dumb
    altitude = int(altitude)

    hold = (altitude & 0x00F)
    altitude = (altitude & 0xFF0) << 1
    altitude = (altitude | hold); #concatenate to the entire message
    altitude = (altitude | 0x010)
    altitude = (altitude | 0x58000) #Takes on the TC (0x58) field

    #formating altitude into bin string
    altitude = str(format(altitude, 'b'))
    
    #print("alt: " +  altitude)

    while (len(altitude) < 20):
        altitude = "0" + altitude

    #Calculate LatLon ------------------------

    #utc bit hardcoded to zero
    utcBit = "0"

    #cpr bit for even odd pairing
    if (odd):
        cpr = 1
        cprBit = "1"
    else:
        cpr = 0
        cprBit = "0"

    #NZ is hardcoded to 15 for our messages
    nz = 15

    #NB is hardcoded to 17 for 17 bits
    nb = 17

    #the latitude zone in the north south direction
    dlat = float(360.0 / (4.0 * nz - cpr))

    #print("dalt: " + str(dlat))

    #latitude for mod operations
    if lat < 0:
        modLat = lat + 360.0
    else:
        modLat = float(lat)

    #the y coordinate within the zone
    #int YZi = (int) Math.round(Math.pow(2, Nb) * ((modLat % Dlat)/ Dlat));
    #yZi = round(math.pow(2,nb)) * (adsbMod(modLat, dlat) / dlat)
    yZi = float(adsbMod(lat, dlat))
    yZi = yZi / dlat
    yZi = yZi * math.pow(2, nb)
    yZi = round(yZi)

    #print("yZi: " + str(yZi))
    

    #the latitude that the receiver will decode
    #rLat = dlat * ((yZi / math.pow(2, nb))) + math.floor(lat / dlat)
    rLat = float(yZi) / math.pow(2, nb)
    rLat = rLat + math.floor(lat / dlat)
    rLat = rLat * dlat

    #print("RLat: " + str(rLat))

    #NLat is based on latitude given
    if (abs(lat) > 87):
        nlLat = 1
    elif (abs(lat) == 87):
        nlLat = 2
    else:
        nlLat = math.floor((2*math.pi)/(math.acos(1 - ((1 - math.cos(math.pi / (2 * nz)))/(math.cos((math.pi / 180) * lat)**2)))))
        #nlLat = math.floor( math.pow * 2 * (math.pow(math.acos(1 - ((1 - math.cos(math.pi / (2 * nz))) / (math.pow(math.cos((math.pi / 180) * abs(float(rLat))), 2)))), -1)) )


    #Longitude calcs

    if ((nlLat - cpr) - cpr) > 0:
        dLon = 360.0 / (nlLat - cpr)
    else:
        dLon = 360.0

    #the X coordinates within the zone
    xZi = adsbMod(lon , dLon)
    xZi = xZi / dLon
    xZi = round(xZi * math.pow(2, nb))

    #make sure the values are nb bits
    yZi = adsbMod(yZi, pow(2,nb))
    xZi = adsbMod(xZi, pow(2, nb))

    #print("yZi: " + str(yZi))
    #print(str(format(int(yZi), 'x')))
    yBin = str(format(int(yZi), 'b'))

    while (len(yBin) < 17):
        yBin = "0" + yBin

    #print("xZi: " + str(xZi))
    #print(str(format(int(xZi), 'x')))
    xBin = str(format(int(xZi), 'b'))

    while (len(xBin) < 17):
        xBin = "0" + xBin

    locMsg = altitude + utcBit + cprBit + yBin + xBin

    #print("bin: " + locMsg)
    #print("hex: " + str(format(int(locMsg,2), 'x')))


    return locMsg

#converts our string into the encoding style needed to transmit
def encodeMsg(msg):
    """
    Mode S encoder that converts the given binary message into the Mode S encoding
    Also adds preamble.

    Returns a binary string of the preamble and encoded message - note needs to transmit at 2Mbps
    """

    #the message preamble to turn on IFF reciever
    preamble = "1010000101000000"

    #end spce to give us room for when we transmit back to back
    endSpace = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"

    encodedMsg = ""

    for i in range(0, len(msg)):
        if (msg[i] == '1'):
            encodedMsg = encodedMsg + "10"
        else:
            encodedMsg = encodedMsg + "01"

    return preamble + encodedMsg + endSpace


if __name__ == '__main__':
    main()