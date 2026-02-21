# mert-os-v1
MorgOS v1 - Türkçe İşletim Sistemi

## GitHub Actions CI Workflow

This repository includes an automated CI workflow that builds the kernel/ISO and captures a screenshot of the booted system.

### How it works

The workflow (`.github/workflows/ci-qemu.yml`) automatically runs when:
- A pull request is opened or updated on the `feature/initial-kernel` or `copilot/feature-initial-kernel` branch
- Manually triggered via workflow dispatch

### What it does

1. **Builds the kernel** using `make all`
2. **Builds the ISO** using `make iso`
3. **Boots the ISO** in QEMU with a headless X virtual framebuffer (xvfb)
4. **Captures a screenshot** of the framebuffer using ImageMagick
5. **Uploads artifacts**:
   - Screenshot (PNG)
   - ISO image
   - QEMU serial logs

### Accessing the artifacts

To view the screenshot and download the ISO:

1. Go to the **Actions** tab in this repository
2. Click on the workflow run you're interested in
3. Scroll down to the **Artifacts** section
4. Download:
   - `qemu-screenshot` - Screenshot of the booted kernel
   - `morgos-iso` - Bootable ISO image
   - `qemu-logs` - Serial output from QEMU (for debugging)

### Notes

- Build failures will cause the job to fail
- Screenshot capture failures are non-fatal and will only show as warnings
- Artifacts are retained for 30 days
- The workflow does NOT automatically publish releases
