# MertOS

x86 mimarisinde sifirdan yazilmis basit bir isletim sistemi.

## Ozellikler

- Multiboot2 uyumlu bootloader (GRUB)
- GDT (Global Descriptor Table) ve IDT (Interrupt Descriptor Table)
- VGA text mode suruculeri (80x25, 16 renk)
- PS/2 keyboard surucusu
- Interaktif shell (komut satiri)
- PIC (Programmable Interrupt Controller) yonetimi

## Shell Komutlari

| Komut    | Aciklama                    |
|----------|-----------------------------|
| help     | Yardim mesajini goster      |
| clear    | Ekrani temizle              |
| info     | Sistem bilgisi              |
| reboot   | Sistemi yeniden baslat      |
| shutdown | Sistemi kapat               |
| time     | CPU tick sayacini goster    |
| echo     | Metni ekrana yaz            |
| color    | Renk testi goster           |
| logo     | MertOS logosunu goster      |
| mem      | Bellek bilgisi              |

## Derleme

### Gereksinimler

```bash
sudo apt-get install nasm gcc make grub-pc-bin grub-common xorriso mtools qemu-system-x86
```

### Build

```bash
make        # Kernel ve ISO olustur
make run    # QEMU ile calistir
make clean  # Temizle
```

## Proje Yapisi

```
mertOS/
├── src/
│   ├── boot/          # Assembly boot kodu
│   │   ├── boot.asm       # Multiboot header ve entry point
│   │   ├── gdt.asm        # GDT flush
│   │   └── interrupt.asm  # ISR ve IRQ handler stubs
│   ├── kernel/        # Kernel C kodu
│   │   ├── kernel.c       # kernel_main entry
│   │   ├── gdt.c/h        # GDT implementasyonu
│   │   └── idt.c/h        # IDT implementasyonu
│   ├── drivers/       # Donanim suruculeri
│   │   ├── vga.c/h        # VGA text mode
│   │   └── keyboard.c/h   # PS/2 keyboard
│   ├── lib/           # Yardimci kutuphaneler
│   │   ├── types.h        # Tip tanimlari
│   │   ├── io.h           # Port I/O
│   │   └── string.c/h     # String fonksiyonlari
│   └── shell/         # Shell implementasyonu
│       └── shell.c/h      # Komut satiri
├── iso/               # GRUB ISO yapisi
├── linker.ld          # Linker script
└── Makefile           # Build sistemi
```

## Ekran Goruntuleri

### Boot Ekrani
OS baslatildiginda MertOS logosu ve karsilama mesaji goruntulenir.

### Shell
Interaktif komut satiri ile sistemi yonetebilirsiniz.
