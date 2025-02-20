import subprocess
import time

def run_avss_command(input_text):
    """Runs the AVSS program with the given input and captures output."""
    process = subprocess.Popen(['./avss'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    output, error = process.communicate(input_text)
    return output, error


def test_registration():
    print("Testing Registration...")
    output, error = run_avss_command("1\nTestUser\nTestPass\n3\n")
    assert "User registered successfully!" in output, "Registration failed!"
    print("✅ Registration Passed")


def test_login():
    print("Testing Login...")
    output, error = run_avss_command("2\nTestUser\nTestPass\n5\n")
    assert "Login successful!" in output, "Login failed!"
    print("✅ Login Passed")


def test_file_creation():
    print("Testing File Creation...")
    output, error = run_avss_command("2\nTestUser\nTestPass\n1\nTestFile.txt\nHello, AVSS!\n5\n")
    assert "File created successfully!" in output, "File creation failed!"
    print("✅ File Creation Passed")


def test_file_listing():
    print("Testing File Listing...")
    output, error = run_avss_command("2\nTestUser\nTestPass\n2\n5\n")
    assert "TestFile.txt" in output, "File listing failed!"
    print("✅ File Listing Passed")


def test_file_reading():
    print("Testing File Reading...")
    output, error = run_avss_command("2\nTestUser\nTestPass\n3\nTestFile.txt\n5\n")
    assert "Hello, AVSS!" in output, "File reading failed!"
    print("✅ File Reading Passed")


def test_file_deletion():
    print("Testing File Deletion...")
    output, error = run_avss_command("2\nTestUser\nTestPass\n4\nTestFile.txt\n5\n")
    assert "File deleted!" in output, "File deletion failed!"
    print("✅ File Deletion Passed")


def run_all_tests():
    test_registration()
    time.sleep(1)
    test_login()
    time.sleep(1)
    test_file_creation()
    time.sleep(1)
    test_file_listing()
    time.sleep(1)
    test_file_reading()
    time.sleep(1)
    test_file_deletion()
    print("\n🎉 All Tests Passed Successfully!")

if __name__ == "__main__":
    run_all_tests()
