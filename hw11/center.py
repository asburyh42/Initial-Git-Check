from PIL import Image
import matplotlib.pyplot as plt

# Load and process the image
img_data = Image.open("line_4.png")
img_data = img_data.convert('RGB')

# Get image dimensions
width, height = img_data.size
print(str(width) + " " + str(height))

# Data lists
index = []
reds = []
greens = []
blues = []
sum = []
line = []  # List to hold values 100 or 0 based on sum average
total_sum = 0
weighted_sum = 0

# Scan through a specific row of pixels in the image
for i in range(width):
    r, g, b = img_data.getpixel((i, 45))  # Assume y=45 is the row you're interested in
    index.append(i)
    reds.append(r)
    greens.append(g)
    blues.append(b)
    pixel_sum = r + g + b
    sum.append(pixel_sum)
    total_sum += pixel_sum
    weighted_sum += pixel_sum * i

# Calculate the average of the sum
average_sum = total_sum / width

# Determine above or below average and set line values
for value in sum:
    if value > average_sum:
        line.append(100)
    else:
        line.append(0)

# Calculate the center of the robot
if total_sum > 0:
    center = weighted_sum / total_sum
else:
    center = None

# Plotting
plt.figure(figsize=(10, 5))  # Set a suitable figure size
plt.plot(index, reds, 'r-', label='Reds')
plt.plot(index, greens, 'g-', label='Greens')
plt.plot(index, blues, 'b-', label='Blues')
plt.plot(index, sum, 'k-', label='Sum')
plt.plot(index, line, 'm-', label='White Line')
plt.axvline(x=center, color='c', label='Center of Robot')  # Adding a vertical line for the center of mass
plt.legend()  # Show legend to identify lines
plt.title('Pixel Intensity and Center')
plt.show()

# Optional: print the center of mass
if center is not None:
    print("Center of Robot:", center)
else:
    print("Total sum is zero, cannot compute center of robot.")