# HideProcessDKOM

Kernel-mode Driver created to simulate a typical hide process DKOM Attack by unlinking the blink and flink, commonly used by rootkits

### Warning #1: altering the kernel data structs may lead to BSOD due to Patchguard protection

### Warning #2: using hardcoded offsets, it may change depending on the version and the build of Windows OS

Usage:

```
sc.exe create hideprocessdkom type= kernel binPath= C:\Path\To\HideProcessDKOM.sys
sc.exe start hideprocessdkom
```