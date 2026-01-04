import os
import string
import random

def makeFiles(path):
    os.makedirs(path, exist_ok=True)
    os.chdir(path)
    for i in range(5):   # Create 5 files
        filename = f"test{i+1}.txt"
        with open(filename, "w") as file:  # Open file in write mode
            # Generate a random string of 1000 characters
            random_string = ''.join(random.choices(string.ascii_uppercase + string.digits, k=1000))
            file.write(random_string)

if __name__ == "__main__":
    makeFiles("test")  