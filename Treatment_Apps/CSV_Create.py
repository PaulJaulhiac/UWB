import serial
import time

# Configure the serial connection
arduino_port = "COM7"  # Replace with your Arduino's COM port
baud_rate = 115200
output_file = "distance_measurements_2m.csv"

# Open the serial port
ser = serial.Serial(arduino_port, baud_rate)
print(f"Connected to {arduino_port} at {baud_rate} baud.")

# Open the file for writing
with open(output_file, "w") as file:
    file.write("ToF,distance,ToF_sk,distance_sk\n")  # Write header row

    try:
        while True:
            # Read a line from the serial port
            if ser.in_waiting > 0:
                line = ser.readline().decode("utf-8").strip()
                print(line)  # Print to console
                file.write(line + "\n")  # Write to file
    except KeyboardInterrupt:
        print("Logging stopped.")
    finally:
        ser.close()

print(f"Data logged to {output_file}")
