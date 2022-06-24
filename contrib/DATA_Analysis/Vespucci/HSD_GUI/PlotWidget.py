from vispy import app, scene
import numpy as np
import math
from collections import deque

from PySide6.QtCore import Qt, Slot, QSize
from PySide6.QtGui import QPainter, QFont, QScreen
from PySide6.QtWidgets import QWidget, QFrame, QVBoxLayout, QPushButton
# from PySide6.QtOpenGLWidgets import QOpenGLWidget
from PySide6.QtUiTools import QUiLoader
from PySide6.QtWidgets import QApplication
from PySide6.QtDesigner import QPyDesignerCustomWidgetCollection

class MyPlotLabel(QWidget):
    def __init__(self, p_label, parent =None):
        super().__init__(parent)
        self.p_label = p_label
    
    def paintEvent(self, event):
        painter = QPainter(self)
        
        # cur_viewport = painter.viewport()
        # painter.setViewport(0,0,35,cur_viewport.height())

        # color = QColor(222, 0, 0)
        # painter.fillRect(painter.viewport(), color)

        # color = QColor(0, 0, 0)
        # color.setNamedColor('#d4d4d4')
        # painter.setPen(color)
        # painter.setBrush(QColor(0, 200, 0))
        # painter.drawRoundedRect(0, 0, 35, painter.viewport().height(), 5, 5)

        # clip = QRect(2, 2, painter.viewport().width() - 4, painter.viewport().height() - 4);
        
        painter.setPen(Qt.white)
        # painter.translate(10, 55)
        painter.translate(painter.viewport().width() - 4, painter.viewport().height() - 4)
        painter.rotate(-90)
        # painter.drawText(clip, self.p_label)
        bold = QFont();
        bold.setBold(True);
        painter.setFont(bold);
        painter.drawText(0, 0, self.p_label.upper())
        painter.end()

# class PlotWidget(QOpenGLWidget):
class PlotWidget(QWidget):
    def __init__(self, controller, comp_name, odr, time_window, n_curves=1, parent=None, p_id = 0):
        super().__init__(parent)
        self.parent = parent
        self.controller = controller
        self.controller.sig_logging.connect(self.s_is_logging)
        
        self.is_docked = True
        self.app_qt = self.controller.qt_app

        self.p_id = p_id
        self.comp_name = comp_name
        
        self.canvas = scene.SceneCanvas(bgcolor="#1b1d23", keys='interactive', vsync=True)
        self.canvas.native.setMinimumSize(QSize(300, 150))
        
        QPyDesignerCustomWidgetCollection.registerCustomWidget(PlotWidget, module="PlotWidget")
        loader = QUiLoader()
        plot_widget = loader.load("HSD_GUI\GUI\plot_widget.ui", parent)
        title_frame = plot_widget.frame_plot.findChild(QFrame,"frame_title")
        contents_frame = plot_widget.frame_plot.findChild(QFrame,"frame_contents")
        pushButton_pop_out = title_frame.findChild(QPushButton, "pushButton_pop_out")
        pushButton_pop_out.clicked.connect(self.clicked_pop_out_button)
        
        #Main layout
        main_layout = QVBoxLayout()
        self.setLayout(main_layout)
        main_layout.addWidget(plot_widget)

        title_frame.layout().addWidget(MyPlotLabel("{}".format(self.comp_name)))
        
        contents_frame.layout().addWidget(self.canvas.native)
        
        grid = self.canvas.central_widget.add_grid(spacing=0)
        self.viewbox = grid.add_view(row=0, col=1, camera='panzoom')

        # add the axes
        x_axis = scene.AxisWidget(orientation='bottom')
        x_axis.stretch = (1, 0.1)
        x_axis.axis.axis_width = x_axis.axis.tick_width = 1
        x_axis.axis.text_color = '#1b1d23' #same as background (hide text)
        x_axis.axis.axis_color = '#d2d2d2'
        x_axis.axis.tick_color = '#666666'
        grid.add_widget(x_axis, row=1, col=1)
        x_axis.link_view(self.viewbox)
        y_axis = scene.AxisWidget(orientation='left')
        y_axis.stretch = (0.1, 1)
        y_axis.axis.axis_width = y_axis.axis.tick_width = 1
        y_axis.axis.text_color = '#d2d2d2'
        y_axis.axis.axis_color = '#d2d2d2'
        y_axis.axis.tick_color = '#666666'
        grid.add_widget(y_axis, row=0, col=0)
        y_axis.link_view(self.viewbox)

        self.positions = dict()
        self._data = dict() # dict of queues

        self.update_plots_ui(odr, time_window, n_curves)

        self.updateTimer = app.Timer()  # Setting up the timer to update UI
        self.updateTimer.connect(self.update_plot)

    def update_plots_ui(self, odr, time_window, n_curves):       
        
        self.positions = dict()
        self._data = dict() # dict of queues
        self.curves = dict()

        self.n_curves = n_curves

        self.downsampling_factor = 1
        self.samples_cnt = int(odr * time_window)
        digits = int(math.log10(self.samples_cnt))+1
        
        #Downsampling based on sensor ODR and time_window (Max plottable point in a time_window = 9999)
        if digits > 4:
            self.downsampling_factor = 10**(digits-4)
        self.N = int(self.samples_cnt/self.downsampling_factor)

        self.color = dict()
        # color arrays
        for i in range(n_curves):
            self.color[i] = np.ones((self.N, 4), dtype=np.float32)
            self.color[i][:, 0] = np.linspace((i/self.n_curves), ((i+1)/self.n_curves), self.N)
            self.color[i][:, 1] = self.color[i][::-1, 0]

        # add a line plot inside the viewbox
        for i in range(self.n_curves):
            self._data[i] = deque(maxlen=self.N)
            self.positions[i] = np.zeros((self.N, 2), dtype='f')#self._dtype_from_data_type(data_type))
            x_lim = [0., self.N]
            self.positions[i][:, 0] = np.linspace(x_lim[0], x_lim[1], self.N)

            self.curves[i] = scene.Line(self.positions[i], self.color[i], method='gl', parent=self.viewbox.scene)
            #TODO TEST
            self.app_qt.processEvents()

        #TODO use FS
        self.min_d = 0
        self.max_d = 10 
        self.viewbox.camera.set_range(x = (0,int(self.N)), y =(self.min_d,self.max_d))

        self.update()

    @Slot()
    def clicked_pop_out_button(self):
        if self.is_docked:
            self.pop_out_widget()
            self.is_docked = False
        else:
            self.pop_in_widget()
            self.is_docked = True

    @Slot(bool)
    def s_is_logging(self, state: bool):
        print("sensor", self.comp_name, "is_logging", state)
        if state:
            self.updateTimer.start(0)
        else:
            self.updateTimer.stop()

    def reset(self):
        pass
        # self._data = deque(maxlen=self._window_size)

        # for c in self._curves:
        #     self._plot.removeItem(c)

        # self._curves = []

        # for i in range(self.n_curves):
        #     self._curves.append(self._plot.plot(pen=pg.mkPen(self.colors[i % len(self.colors)], width=1),connect='auto'))
        #     self._curves[i].setDownsampling(auto=True, method='peak')
        #     self._curves[i].setClipToView(True)

    def update_plot(self, ev):       
        if len(self._data[0]) > 0:
            max_d_now = []
            min_d_now = []
            for i in range(self.n_curves):
                if len(self._data[i]) < self.N:
                    a = self.positions[i][:(self.N - len(self._data[i])),1]
                    self.positions[i][:, 1] = np.append(a,np.array(self._data[i]))
                else:    
                    self.positions[i][:, 1] = np.array(self._data[i])

                self.curves[i].set_data(pos=self.positions[i], color=self.color[i])
                
                #TODO to be improved
                min_d_now.append(np.amin(self.positions[i][:, 1]))
                max_d_now.append(np.amax(self.positions[i][:, 1]))
            
            max_data = np.amax(max_d_now)
            min_data = np.amin(min_d_now)
            if min_data < self.min_d:
                self.viewbox.camera.set_range(x=(0,self.N),y=(min_data, self.max_d))
                self.min_d = min_data
            if max_data > self.max_d:
                self.viewbox.camera.set_range(x=(0,self.N),y=(self.min_d, max_data))
                self.max_d = max_data
                
            self.app_qt.processEvents()

    def add_data(self, data):
        # if len(data) > 0:
        for i in range(self.n_curves):
            # Downsampling for PLOTs
            self._data[i].extend(data[i][0:data[i].size:self.downsampling_factor])

    def closeEvent(self, event):
        event.accept()

    def closeEvent(self, event):
        self.pop_in_widget()
        self.is_docked = True

    def pop_out_widget(self):
        self.setWindowFlags(Qt.Dialog)
        center = QScreen.availableGeometry(QApplication.primaryScreen()).center()
        geo = self.frameGeometry()
        geo.moveCenter(center)
        self.move(geo.topLeft())
        self.show()

    def pop_in_widget(self):
        self.setWindowFlags(Qt.Widget)
        self.parent.layout().insertWidget(self.p_id, self)