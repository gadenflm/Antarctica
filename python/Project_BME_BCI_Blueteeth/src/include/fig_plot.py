import matplotlib
matplotlib.use("Qt5Agg")
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as figure_canvas
from matplotlib.figure import Figure


class fig_plot(figure_canvas):
    def __init__(self, width=5, height=4, dpi=100):
        # 第一步：创建一个创建Figure
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        # 第二步：在父类中激活Figure窗口
        super(fig_plot, self).__init__(self.fig)
        self.axes1=self.fig.add_subplot(811)
        self.axes2 = self.fig.add_subplot(812)
        self.axes3 = self.fig.add_subplot(813)
        self.axes4 = self.fig.add_subplot(814)
        self.axes5 = self.fig.add_subplot(815)
        self.axes6 = self.fig.add_subplot(816)
        self.axes7 = self.fig.add_subplot(817)
        self.axes8 = self.fig.add_subplot(818)