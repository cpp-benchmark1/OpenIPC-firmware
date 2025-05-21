# CWE-134 Test Guide

This guide explains how to build, run and test the CWE-134 (Format String) vulnerabilities in the xmdp project.

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

## Testing CWE-134

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

4. Run the exploit with different format strings:
```bash
# Memory leak example
python3 exploit_cwe134.py 127.0.0.1 34569 '%x%x%x%x'

# Write to memory example
python3 exploit_cwe134.py 127.0.0.1 34569 '%n%n%n%n'

# String format example
python3 exploit_cwe134.py 127.0.0.1 34569 '%s%s%s%s'
```

## Expected Results

When running the exploit, you should see:

1. In Terminal 1 (xmdp):
   - The payload being received
   - For Example 1: Direct format string output from printf()
   - For Example 2: Processed output from snprintf()

2. In Terminal 2 (exploit):
   - The exploit script should complete without waiting for response

## Understanding the Vulnerabilities

The project demonstrates two format string vulnerabilities:

### Example 1: Direct Format String
- SOURCE: User input from `UserName` field via recvfrom()
- Transformations:
  1. Convert to uppercase
- SINK: `printf()` with user-controlled format string
- Flow: Single function chain

### Example 2: Indirect Format String
- SOURCE: User input from `PassWord` field via recvfrom()
- Transformations:
  1. Replace spaces with underscores
  2. Add "user_" prefix
- SINK: `snprintf()` with user-controlled format string
- Flow: Cross-function with multiple transformations

## Troubleshooting

If you encounter issues:

1. Make sure xmdp is running in the first terminal
2. Verify the container is running: `docker ps`
3. Check if the ports are accessible: `netstat -tulpn | grep 34569`
4. Ensure you have proper permissions in the container

## Notes

- The exploit demonstrates two format string vulnerabilities:
  1. Direct format string through `UserName`
  2. Indirect format string through `PassWord`
- The payload is sent via UDP broadcast
- No response is expected from the exploit script
- The vulnerabilities are intentionally left unpatched for testing purposes
- Both examples use realistic transformations that don't prevent format string injection 