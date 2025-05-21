# CWE-78 Test Guide

This guide explains how to build, run and test the CWE-78 (Command Injection) vulnerability in the xmdp project.

## Prerequisites

- Docker installed
- Git installed
- Basic knowledge of Linux commands

## Building the Project

1. Build the Docker image:
```bash
docker build -t openipc-firmware .
```

2. Run the container:
```bash
docker run -it --name openipc-test openipc-firmware
```

## Running xmdp

1. Inside the container, navigate to the xmdp directory:
```bash
cd /root/xmdp/general/package/xmdp/src
```

2. Build xmdp:
```bash
make clean && make
```

3. Run xmdp:
```bash
./xmdp
```

## Testing CWE-78

The test will be performed in two terminals:

### Terminal 1 (xmdp)
1. Run xmdp as described above
2. Keep it running to receive the payload

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
python3 exploit_cwe78.py 127.0.0.1 34569 "cat /etc/passwd"
```

## Expected Results

When running the exploit, you should see:

1. In Terminal 1 (xmdp):
   - The payload being received
   - Command execution errors (expected)
   - Output from the executed command

2. In Terminal 2 (exploit):
   - The exploit script should complete without waiting for response

## Troubleshooting

If you encounter issues:

1. Make sure xmdp is running in the first terminal
2. Verify the container is running: `docker ps`
3. Check if the ports are accessible: `netstat -tulpn | grep 34569`
4. Ensure you have proper permissions in the container

## Notes

- The exploit demonstrates two command injection vulnerabilities:
  1. Direct command injection through `UserName`
  2. Command injection through `PassWord` with transformations
- The payload is sent via UDP broadcast
- No response is expected from the exploit script
- The vulnerabilities are intentionally left unpatched for testing purposes 