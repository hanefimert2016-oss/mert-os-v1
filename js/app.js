// ===== Global Variables =====
let windowZIndex = 100;
let activeWindows = new Map();

// ===== Initialization =====
document.addEventListener('DOMContentLoaded', () => {
    initSplashScreen();
    initDock();
    initTopBar();
});

// ===== Splash Screen =====
function initSplashScreen() {
    setTimeout(() => {
        const splashScreen = document.getElementById('splashScreen');
        const desktop = document.getElementById('desktop');
        
        splashScreen.style.animation = 'fadeOut 0.5s ease-out';
        splashScreen.addEventListener('animationend', () => {
            splashScreen.classList.add('hidden');
            desktop.classList.remove('hidden');
        });
    }, 2500);
}

// ===== Top Bar =====
function initTopBar() {
    updateTime();
    setInterval(updateTime, 1000);
}

function updateTime() {
    const now = new Date();
    
    // Update time
    const timeElement = document.getElementById('currentTime');
    const hours = String(now.getHours()).padStart(2, '0');
    const minutes = String(now.getMinutes()).padStart(2, '0');
    timeElement.textContent = `${hours}:${minutes}`;
    
    // Update date
    const dateElement = document.getElementById('currentDate');
    const options = { day: 'numeric', month: 'long', year: 'numeric' };
    dateElement.textContent = now.toLocaleDateString('tr-TR', options);
}

// ===== Dock =====
function initDock() {
    const dockItems = document.querySelectorAll('.dock-item');
    
    dockItems.forEach(item => {
        item.addEventListener('click', () => {
            const appName = item.getAttribute('data-app');
            openApplication(appName);
        });
        
        // Accessibility
        item.setAttribute('role', 'button');
        item.setAttribute('tabindex', '0');
        item.setAttribute('aria-label', item.getAttribute('title'));
        
        // Keyboard support
        item.addEventListener('keydown', (e) => {
            if (e.key === 'Enter' || e.key === ' ') {
                e.preventDefault();
                const appName = item.getAttribute('data-app');
                openApplication(appName);
            }
        });
    });
}

// ===== Window Management =====
function openApplication(appName) {
    // Check if window already exists
    if (activeWindows.has(appName)) {
        const existingWindow = activeWindows.get(appName);
        bringToFront(existingWindow);
        return;
    }
    
    const windowContent = getApplicationContent(appName);
    const windowTitle = getApplicationTitle(appName);
    
    createWindow(appName, windowTitle, windowContent);
}

function getApplicationTitle(appName) {
    const titles = {
        'file-manager': 'Dosya Yöneticisi',
        'calendar': 'Takvim',
        'text-editor': 'Metin Editörü',
        'terminal': 'Terminal',
        'settings': 'Ayarlar'
    };
    return titles[appName] || 'Uygulama';
}

function getApplicationContent(appName) {
    switch (appName) {
        case 'file-manager':
            return createFileManagerContent();
        case 'calendar':
            return createCalendarContent();
        case 'text-editor':
            return createTextEditorContent();
        case 'terminal':
            return createTerminalContent();
        case 'settings':
            return createSettingsContent();
        default:
            return '<p>Uygulama içeriği yükleniyor...</p>';
    }
}

// ===== Application Contents =====

function createFileManagerContent() {
    return `
        <div class="file-manager">
            <h3>Klasörler ve Dosyalar</h3>
            <ul class="file-tree">
                <li class="file-item folder" onclick="toggleFolder(this)">
                    Belgeler
                    <ul class="file-tree hidden">
                        <li class="file-item file">Rapor.pdf</li>
                        <li class="file-item file">Notlar.txt</li>
                    </ul>
                </li>
                <li class="file-item folder" onclick="toggleFolder(this)">
                    Resimler
                    <ul class="file-tree hidden">
                        <li class="file-item file">Tatil.jpg</li>
                        <li class="file-item file">Profil.png</li>
                    </ul>
                </li>
                <li class="file-item folder" onclick="toggleFolder(this)">
                    Müzikler
                    <ul class="file-tree hidden">
                        <li class="file-item file">Şarkı1.mp3</li>
                        <li class="file-item file">Şarkı2.mp3</li>
                    </ul>
                </li>
                <li class="file-item folder" onclick="toggleFolder(this)">
                    Videolar
                    <ul class="file-tree hidden">
                        <li class="file-item file">Film.mp4</li>
                    </ul>
                </li>
                <li class="file-item file">Dosya.txt</li>
            </ul>
        </div>
    `;
}

function createCalendarContent() {
    const now = new Date();
    const dayNames = ['Pazar', 'Pazartesi', 'Salı', 'Çarşamba', 'Perşembe', 'Cuma', 'Cumartesi'];
    const monthNames = ['Ocak', 'Şubat', 'Mart', 'Nisan', 'Mayıs', 'Haziran', 
                        'Temmuz', 'Ağustos', 'Eylül', 'Ekim', 'Kasım', 'Aralık'];
    
    const dayName = dayNames[now.getDay()];
    const day = now.getDate();
    const month = monthNames[now.getMonth()];
    const year = now.getFullYear();
    
    return `
        <div class="calendar-container">
            <div class="calendar-header">📅 Takvim</div>
            <div class="calendar-date">${day}</div>
            <div class="calendar-info">
                <p><strong>${dayName}</strong></p>
                <p>${month} ${year}</p>
            </div>
        </div>
    `;
}

function createTextEditorContent() {
    return `
        <div class="text-editor-container">
            <div class="editor-toolbar">
                <button class="editor-btn" onclick="execCommand('bold')">Kalın</button>
                <button class="editor-btn" onclick="execCommand('italic')">İtalik</button>
                <button class="editor-btn" onclick="clearEditor()">Temizle</button>
            </div>
            <textarea class="editor-textarea" id="editorArea" placeholder="Buraya yazın..."></textarea>
        </div>
    `;
}

function createTerminalContent() {
    return `
        <div class="terminal-container">
            <div class="terminal-output" id="terminalOutput">
                <div class="terminal-line">MorgOS Terminal v1.0</div>
                <div class="terminal-line">Hoş geldiniz! Komut girmek için aşağıdaki alanı kullanın.</div>
                <div class="terminal-line">Kullanılabilir komutlar: help, clear, date, about</div>
                <div class="terminal-line">---</div>
            </div>
            <div class="terminal-input-container">
                <span class="terminal-prompt">morgos@user:~$</span>
                <input type="text" class="terminal-input" id="terminalInput" autocomplete="off" />
            </div>
        </div>
    `;
}

function createSettingsContent() {
    return `
        <div class="settings-container">
            <div class="settings-section">
                <div class="settings-title">🎨 Tema</div>
                <div class="settings-option">
                    <span>Renk Şeması:</span>
                    <select class="settings-select" onchange="changeTheme(this.value)">
                        <option value="light">Açık</option>
                        <option value="dark">Koyu</option>
                        <option value="auto">Otomatik</option>
                    </select>
                </div>
            </div>
            <div class="settings-section">
                <div class="settings-title">🌍 Dil</div>
                <div class="settings-option">
                    <span>Sistem Dili:</span>
                    <select class="settings-select" onchange="changeLanguage(this.value)">
                        <option value="tr" selected>Türkçe</option>
                        <option value="en">English</option>
                        <option value="de">Deutsch</option>
                    </select>
                </div>
            </div>
            <div class="settings-section">
                <div class="settings-title">ℹ️ Hakkında</div>
                <p><strong>MorgOS</strong> v1.0</p>
                <p>Modern Türkçe İşletim Sistemi Arayüzü</p>
                <p>© 2026 MorgOS</p>
            </div>
        </div>
    `;
}

// ===== Window Creation =====
function createWindow(appName, title, content) {
    const windowsContainer = document.getElementById('windowsContainer');
    
    const window = document.createElement('div');
    window.className = 'window';
    window.style.zIndex = ++windowZIndex;
    window.setAttribute('data-app', appName);
    
    // Random position
    const maxX = window.offsetWidth || 400;
    const maxY = window.offsetHeight || 300;
    const randomX = Math.max(50, Math.min(window.innerWidth - maxX - 50, Math.random() * 300 + 100));
    const randomY = Math.max(100, Math.min(window.innerHeight - maxY - 150, Math.random() * 200 + 100));
    
    window.style.left = randomX + 'px';
    window.style.top = randomY + 'px';
    
    window.innerHTML = `
        <div class="window-header">
            <div class="window-title">${title}</div>
            <div class="window-controls">
                <button class="window-control-btn close" onclick="closeWindow('${appName}')" aria-label="Kapat"></button>
                <button class="window-control-btn minimize" onclick="minimizeWindow('${appName}')" aria-label="Küçült"></button>
                <button class="window-control-btn maximize" onclick="maximizeWindow('${appName}')" aria-label="Büyüt"></button>
            </div>
        </div>
        <div class="window-content">
            ${content}
        </div>
    `;
    
    windowsContainer.appendChild(window);
    activeWindows.set(appName, window);
    
    // Make window draggable
    makeWindowDraggable(window);
    
    // Initialize app-specific features
    initializeAppFeatures(appName);
    
    // Click to bring to front
    window.addEventListener('click', () => bringToFront(window));
}

function closeWindow(appName) {
    const window = activeWindows.get(appName);
    if (window) {
        window.style.animation = 'windowClose 0.2s ease-out';
        window.addEventListener('animationend', () => {
            window.remove();
            activeWindows.delete(appName);
        });
    }
}

function minimizeWindow(appName) {
    const window = activeWindows.get(appName);
    if (window) {
        window.style.transform = 'scale(0)';
        window.style.opacity = '0';
        window.style.transition = 'all 0.3s';
        setTimeout(() => {
            window.style.display = 'none';
        }, 300);
    }
}

function maximizeWindow(appName) {
    const window = activeWindows.get(appName);
    if (window) {
        if (window.style.width === '90vw') {
            // Restore
            window.style.width = '';
            window.style.height = '';
            window.style.top = '';
            window.style.left = '';
        } else {
            // Maximize
            window.style.width = '90vw';
            window.style.height = 'calc(100vh - 160px)';
            window.style.top = '50px';
            window.style.left = '5vw';
        }
    }
}

function bringToFront(window) {
    window.style.zIndex = ++windowZIndex;
}

// ===== Window Dragging =====
function makeWindowDraggable(window) {
    const header = window.querySelector('.window-header');
    let isDragging = false;
    let currentX;
    let currentY;
    let initialX;
    let initialY;
    
    header.addEventListener('mousedown', dragStart);
    document.addEventListener('mousemove', drag);
    document.addEventListener('mouseup', dragEnd);
    
    function dragStart(e) {
        if (e.target.classList.contains('window-control-btn')) return;
        
        initialX = e.clientX - window.offsetLeft;
        initialY = e.clientY - window.offsetTop;
        isDragging = true;
        window.style.cursor = 'move';
    }
    
    function drag(e) {
        if (isDragging) {
            e.preventDefault();
            currentX = e.clientX - initialX;
            currentY = e.clientY - initialY;
            
            window.style.left = currentX + 'px';
            window.style.top = currentY + 'px';
        }
    }
    
    function dragEnd() {
        isDragging = false;
        window.style.cursor = 'default';
    }
}

// ===== App-Specific Features =====

function initializeAppFeatures(appName) {
    if (appName === 'terminal') {
        initTerminal();
    }
}

function initTerminal() {
    setTimeout(() => {
        const input = document.getElementById('terminalInput');
        if (input) {
            input.focus();
            input.addEventListener('keydown', handleTerminalCommand);
        }
    }, 100);
}

function handleTerminalCommand(e) {
    if (e.key === 'Enter') {
        const input = e.target;
        const command = input.value.trim();
        const output = document.getElementById('terminalOutput');
        
        if (command) {
            output.innerHTML += `<div class="terminal-line"><span class="terminal-prompt">morgos@user:~$</span> ${command}</div>`;
            
            const response = executeCommand(command);
            output.innerHTML += `<div class="terminal-line">${response}</div>`;
            
            input.value = '';
            output.scrollTop = output.scrollHeight;
        }
    }
}

function executeCommand(command) {
    const cmd = command.toLowerCase();
    
    switch (cmd) {
        case 'help':
            return 'Kullanılabilir komutlar:\n  help - Yardım\n  clear - Ekranı temizle\n  date - Tarih ve saat\n  about - MorgOS hakkında';
        case 'clear':
            setTimeout(() => {
                const output = document.getElementById('terminalOutput');
                if (output) output.innerHTML = '';
            }, 100);
            return '';
        case 'date':
            return new Date().toLocaleString('tr-TR');
        case 'about':
            return 'MorgOS v1.0 - Modern Türkçe İşletim Sistemi Arayüzü';
        default:
            return `Komut bulunamadı: ${command}`;
    }
}

// File Manager
function toggleFolder(element) {
    const subTree = element.querySelector('.file-tree');
    if (subTree) {
        subTree.classList.toggle('hidden');
    }
}

// Text Editor
function execCommand(command) {
    const textarea = document.getElementById('editorArea');
    if (!textarea) return;
    
    const start = textarea.selectionStart;
    const end = textarea.selectionEnd;
    const selectedText = textarea.value.substring(start, end);
    
    if (command === 'bold') {
        const newText = `**${selectedText}**`;
        textarea.value = textarea.value.substring(0, start) + newText + textarea.value.substring(end);
    } else if (command === 'italic') {
        const newText = `*${selectedText}*`;
        textarea.value = textarea.value.substring(0, start) + newText + textarea.value.substring(end);
    }
}

function clearEditor() {
    const textarea = document.getElementById('editorArea');
    if (textarea) {
        textarea.value = '';
    }
}

// Settings
function changeTheme(theme) {
    alert(`Tema değiştirildi: ${theme}`);
}

function changeLanguage(lang) {
    alert(`Dil değiştirildi: ${lang}`);
}

// ===== Additional Animations =====
const style = document.createElement('style');
style.textContent = `
    @keyframes fadeOut {
        from { opacity: 1; }
        to { opacity: 0; }
    }
    @keyframes windowClose {
        from { transform: scale(1); opacity: 1; }
        to { transform: scale(0.8); opacity: 0; }
    }
`;
document.head.appendChild(style);
