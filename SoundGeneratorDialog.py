import numpy as np
import scipy.io.wavfile as wav
from scipy.signal import chirp
from PyQt5.QtWidgets import QApplication, QDialog, QLabel, QSpinBox, QPushButton, QVBoxLayout, QLineEdit, QHBoxLayout, QMessageBox


class SoundGeneratorDialog(QDialog):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("次声波生成器")
        layout = QVBoxLayout()

        self.label_duration = QLabel("持续时间（秒）：")
        self.spin_duration = QSpinBox()
        self.spin_duration.setRange(1, 10)
        self.spin_duration.setValue(3)

        self.label_frequency = QLabel("频率（Hz）：")
        self.spin_frequency = QSpinBox()
        self.spin_frequency.setRange(20, 20000)
        self.spin_frequency.setValue(100)

        self.label_filename = QLabel("文件名：")
        self.edit_filename = QLineEdit()

        self.button_generate = QPushButton("生成次声波")
        self.button_generate.clicked.connect(self.generate_sound)

        layout.addWidget(self.label_duration)
        layout.addWidget(self.spin_duration)
        layout.addWidget(self.label_frequency)
        layout.addWidget(self.spin_frequency)
        layout.addWidget(self.label_filename)
        layout.addWidget(self.edit_filename)
        layout.addWidget(self.button_generate)

        self.setLayout(layout)

    def generate_sound(self):
        duration = self.spin_duration.value()
        frequency = self.spin_frequency.value()
        filename = self.edit_filename.text()

        if not filename:
            QMessageBox.warning(self, "警告", "请输入文件名")
            return

        # 生成次声波数据
        fs = 44100  # 采样率
        t = np.linspace(0, duration, int(fs * duration), endpoint=False)
        samples = chirp(t, f0=20, f1=frequency, t1=duration, method='quadratic')

        # 归一化
        samples = samples * (32767 / np.max(np.abs(samples)))

        # 保存为.wav文件
        filename = filename.strip() + ".wav"
        wav.write(filename, fs, samples.astype(np.int16))
        QMessageBox.information(self, "提示", f"次声波文件已保存为 {filename}")


if __name__ == "__main__":
    app = QApplication([])
    dialog = SoundGeneratorDialog()
    dialog.exec_()
