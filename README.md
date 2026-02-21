# MorgOS v1 - Modern Türkçe İşletim Sistemi Arayüzü

<div align="center">
  <img src="assets/logo.svg" width="150" alt="MorgOS Logo">
  <h3>🇹🇷 Tamamen Türkçe | Modern | Kullanıcı Dostu</h3>
</div>

## 📖 Tanıtım

MorgOS v1, modern web teknolojileri kullanılarak geliştirilmiş, tamamen Türkçe bir işletim sistemi arayüzü prototipidir. macOS ve Windows'un en iyi özelliklerini birleştiren bu proje, Glass Morphism tasarım trendini kullanarak görsel olarak çarpıcı bir deneyim sunar.

## ✨ Özellikler

### 🎨 Tasarım
- **Glass Morphism**: Şeffaf ve bulanık efektler ile modern görünüm
- **Renk Şeması**: Açık mavi (#87CEEB) ana renk tonu
- **Responsive**: Mobil ve masaüstü cihazlarda mükemmel çalışır
- **Animasyonlar**: Yumuşak geçişler ve hover efektleri
- **macOS + Windows Karışımı**: Yuvarlatılmış köşeler ve modern düz tasarım

### 🚀 Uygulamalar

1. **📁 Dosya Yöneticisi**
   - Klasör ve dosya yapısını görüntüleme
   - Genişletilebilir klasör ağacı
   - Kolay navigasyon

2. **📅 Takvim**
   - Güncel tarih ve saat gösterimi
   - Türkçe ay ve gün isimleri
   - Temiz ve okunabilir tasarım

3. **📝 Metin Editörü**
   - Yazı yazma ve düzenleme
   - Kalın/İtalik format seçenekleri
   - Temizleme fonksiyonu

4. **💻 Terminal**
   - Komut satırı arayüzü
   - Temel komutlar (help, clear, date, about)
   - Gerçek zamanlı komut çalıştırma

5. **⚙️ Ayarlar**
   - Tema seçenekleri
   - Dil değiştirme
   - Sistem bilgileri

### 🎯 İşlevsellik

- **Dock Sistemi**: Altta yer alan, hover efektli uygulama başlatıcı
- **Pencere Yönetimi**: Sürüklenebilir, kapatılabilir, küçültülebilir pencereler
- **Başlangıç Animasyonu**: Logo ve yükleme ekranı
- **Çoklu Pencere Desteği**: Aynı anda birden fazla uygulama açabilme
- **Türkçe Arayüz**: Tüm menüler ve mesajlar Türkçe

## 🛠️ Kurulum

### Gereksinimler
- Modern bir web tarayıcısı (Chrome, Firefox, Safari, Edge)
- Yerel bir web sunucusu (opsiyonel)

### Adımlar

1. **Repository'yi klonlayın**
   ```bash
   git clone https://github.com/hanefimert2016-oss/mert-os-v1.git
   cd mert-os-v1
   ```

2. **Doğrudan açın**
   - `index.html` dosyasını çift tıklayarak tarayıcınızda açın

   VEYA

3. **Yerel sunucu ile çalıştırın** (Önerilen)
   ```bash
   # Python ile
   python -m http.server 8000
   
   # Node.js ile (http-server gereklidir)
   npx http-server
   
   # PHP ile
   php -S localhost:8000
   ```

4. **Tarayıcınızda açın**
   ```
   http://localhost:8000
   ```

## 📁 Dosya Yapısı

```
mert-os-v1/
├── index.html              # Ana HTML dosyası
├── css/
│   └── style.css          # Tüm stiller ve animasyonlar
├── js/
│   └── app.js             # Uygulama mantığı ve fonksiyonlar
├── assets/
│   ├── logo.svg           # MorgOS logosu
│   └── icons/             # Uygulama ikonları
│       ├── file-manager.svg
│       ├── calendar.svg
│       ├── text-editor.svg
│       ├── terminal.svg
│       └── settings.svg
└── README.md              # Bu dosya
```

## 🎨 Renk Paleti

| Renk | Hex Kodu | Kullanım |
|------|----------|----------|
| Ana Renk | `#87CEEB` | Logolar, vurgular |
| Arka Plan | `#F0F4F8` | Ana arka plan |
| Dock | `rgba(255, 255, 255, 0.7)` | Dock arka planı |
| Metin | `#333333` | Ana metin rengi |
| Vurgu | `#4A90E2` | Hover efektleri |

## 🖥️ Tarayıcı Desteği

- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+
- ✅ Opera 76+

## 📱 Responsive Tasarım

MorgOS v1, farklı ekran boyutlarına otomatik olarak uyum sağlar:
- 📱 Mobil cihazlar (< 480px)
- 📱 Tablet cihazlar (480px - 768px)
- 💻 Masaüstü (> 768px)

## 🔧 Teknik Detaylar

### Kullanılan Teknolojiler
- **HTML5**: Semantik yapı
- **CSS3**: Flexbox, Grid, Backdrop Filter, Animasyonlar
- **JavaScript (Vanilla)**: Framework kullanılmadan saf JS
- **SVG**: Ölçeklendirilebilir vektör grafikleri

### Özellikler
- Glass Morphism (backdrop-filter)
- Smooth Animations (CSS transitions & keyframes)
- Drag & Drop (Pencere sürükleme)
- Event Handling (Klavye ve fare olayları)
- DOM Manipulation (Dinamik içerik oluşturma)
- Responsive Design (Media queries)

## 🎮 Kullanım

### Dock'tan Uygulama Başlatma
1. Alttaki dock'taki herhangi bir ikona tıklayın
2. Uygulama penceresi açılacaktır
3. İstediğiniz kadar uygulama açabilirsiniz

### Pencere İşlemleri
- **Sürükleme**: Pencere başlığından tutup sürükleyin
- **Kapatma**: Kırmızı butona tıklayın
- **Küçültme**: Sarı butona tıklayın
- **Büyütme**: Yeşil butona tıklayın

### Terminal Komutları
- `help` - Yardım mesajını gösterir
- `clear` - Ekranı temizler
- `date` - Tarih ve saati gösterir
- `about` - MorgOS hakkında bilgi

## 🚧 Geliştirme Durumu

- [x] Temel tasarım ve yapı
- [x] Dock sistemi
- [x] Başlangıç animasyonu
- [x] 5 temel uygulama
- [x] Pencere yönetimi
- [x] Responsive tasarım
- [x] Türkçe arayüz
- [ ] Daha fazla uygulama
- [ ] Dosya sistemi API'si
- [ ] Kullanıcı hesapları
- [ ] Tema özelleştirme
- [ ] Widget desteği

## 🤝 Katkıda Bulunma

Katkılarınızı bekliyoruz! Lütfen aşağıdaki adımları izleyin:

1. Bu repository'yi fork edin
2. Yeni bir branch oluşturun (`git checkout -b feature/AmazingFeature`)
3. Değişikliklerinizi commit edin (`git commit -m 'Add some AmazingFeature'`)
4. Branch'inizi push edin (`git push origin feature/AmazingFeature`)
5. Bir Pull Request açın

## 📄 Lisans

Bu proje açık kaynak kodludur ve serbestçe kullanılabilir.

## 👨‍💻 Geliştirici

MorgOS v1 - 2026

## 🌟 Teşekkürler

Bu projeyi beğendiyseniz, yıldız vermeyi unutmayın! ⭐

## 📞 İletişim

Sorularınız veya önerileriniz için Issue açabilirsiniz.

---

<div align="center">
  Made with ❤️ in Turkey 🇹🇷
</div>
