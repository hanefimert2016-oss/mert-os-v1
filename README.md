# MorgOS v1 - Minimal x86_64 İşletim Sistemi

MorgOS, sıfırdan yazılmış, x86_64 mimarisi için özel bootloader ve minimal kernel içeren açık kaynak bir işletim sistemi projesidir.

## 🎯 Proje Özeti

Bu proje, UEFI/BIOS uyumlu (UEFI öncelikli) özel bir bootloader ve temel kernel içerir:
- **Custom Bootloader**: 32-bit korumalı mod → 64-bit long mode geçişi (Multiboot2/GRUB uyumlu)
- **Minimal Kernel**: GDT/IDT kurulumu, sayfalama, VGA/framebuffer çıktısı
- **Basit Init Sistemi**: Kernel başlangıç betiği

## 📁 Proje Yapısı

```
mert-os-v1/
├── LICENSE               # MIT lisansı
├── README.md            # Bu dosya
├── Makefile             # Derleme sistemi
├── .gitignore           # Git yoksayma kuralları
├── src/
│   ├── boot/
│   │   ├── boot.S       # Assembly bootloader (16→32→64 bit geçişi)
│   │   └── linker.ld    # Kernel linker script
│   └── kernel/
│       ├── main.c       # Ana kernel kodu
│       └── ksys.c       # Kernel yardımcı fonksiyonları
├── initramfs/
│   └── init             # Init betiği
└── tools/
    └── build-iso.sh     # ISO oluşturma scripti
```

## 🛠️ Gereksinimler

Projeyi derlemek için aşağıdaki araçlara ihtiyacınız var:

- **GNU Make**: Derleme otomasyonu
- **GCC Cross-Compiler**: x86_64-elf-gcc (veya uyumlu cross-compiler)
- **NASM veya GNU Assembler**: Assembly kodu derlemek için
- **xorriso**: ISO görüntüsü oluşturmak için (isteğe bağlı)
- **GRUB**: GRUB bootloader (grub-mkrescue, isteğe bağlı)
- **QEMU**: Test için (qemu-system-x86_64)

### Cross-Compiler Kurulumu

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential nasm xorriso grub-pc-bin grub-common qemu-system-x86
```

**macOS:**
```bash
brew install x86_64-elf-gcc nasm xorriso qemu
```

**Manuel Cross-Compiler** (gerekirse):
- [OSDev Wiki](https://wiki.osdev.org/GCC_Cross-Compiler) adresindeki talimatları izleyin

## 🚀 Derleme

### Tüm Hedefleri Derle
```bash
make all
```

Bu komut:
1. Bootloader'ı derler (boot.S → boot.o)
2. Kernel'i derler (main.c, ksys.c → kernel.elf)
3. Çıktıları `build/` dizinine yerleştirir

### Temizlik
```bash
make clean
```

### ISO Görüntüsü Oluşturma (İsteğe Bağlı)
```bash
make iso
```

Bu komut `build/morgos.iso` dosyasını oluşturur. GRUB ve xorriso kurulu olmalıdır.

## 🧪 Test Etme

### QEMU ile Lokal Test

**ISO dosyasından başlatma:**
```bash
qemu-system-x86_64 -cdrom build/morgos.iso -m 1024
```

**Doğrudan kernel ELF'den başlatma (hızlı test):**
```bash
qemu-system-x86_64 -kernel build/kernel.elf -m 1024
```

### Beklenen Çıktı

Kernel başarıyla yüklendiğinde, ekranda şuna benzer bir çıktı görmelisiniz:
```
MorgOS kernel alive
Kernel initialized successfully
```

### VirtualBox ile Test (İsteğe Bağlı)

1. VirtualBox'ta yeni bir VM oluşturun (Type: Other, Version: Other/Unknown 64-bit)
2. Ayarlar → Depolama → ISO dosyasını ekleyin
3. VM'yi başlatın

## 📝 Geliştirme Notları

### İlk Sürüm Özellikleri (v0.1)

- ✅ 64-bit long mode geçişi
- ✅ Temel GDT (Global Descriptor Table) kurulumu
- ✅ Temel IDT (Interrupt Descriptor Table) kurulumu
- ✅ Sayfalama etkin
- ✅ VGA text mode çıktısı
- ✅ Basit printk/console fonksiyonları

### Gelecek Özellikler (Planlanıyor)

- 🔲 Gelişmiş interrupt handling
- 🔲 Klavye girişi
- 🔲 Dosya sistemi (VFS)
- 🔲 Kullanıcı modu geçişi
- 🔲 Çoklu görev (multitasking)
- 🔲 Grafik modu desteği
- 🔲 Network stack (HTTP client)
- 🔲 Basit shell ve kullanıcı uygulamaları

## 🤝 Katkıda Bulunma

1. Bu repo'yu fork edin
2. Feature branch oluşturun (`git checkout -b feature/amazing-feature`)
3. Değişikliklerinizi commit edin (`git commit -m 'Add some amazing feature'`)
4. Branch'inizi push edin (`git push origin feature/amazing-feature`)
5. Pull Request açın

## 📄 Lisans

Bu proje MIT lisansı altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.

## 📚 Kaynaklar

- [OSDev Wiki](https://wiki.osdev.org/) - İşletim sistemi geliştirme kaynakları
- [Intel 64 and IA-32 Architectures Software Developer Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
- [AMD64 Architecture Programmer's Manual](https://www.amd.com/en/support/tech-docs)

## 👤 Yazar

**MorgOS Project**
- GitHub: [@hanefimert2016-oss](https://github.com/hanefimert2016-oss)

---

**Not**: Bu proje eğitim ve öğrenme amaçlıdır. Production kullanımı için uygun değildir.
