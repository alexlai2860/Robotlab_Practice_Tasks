import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms
torch.__version__

class ConvNet(nn.Module):
    def __init__(self):
        super().__init__()
        # batch*1*28*28（每次会送入batch个样本，输入通道数1（黑白图像），图像分辨率是28x28
        self.conv1 = nn.Conv2d(1, 6, 5, padding=2)  # 输入通道数1，输出通道数6，核的大小5
        self.conv2 = nn.Conv2d(6, 16, 5)  # 输入通道数6，输出通道数16，核的大小5
        # 下面的全连接层Linear的第一个参数指输入通道数，第二个参数指输出通道数
        self.fc1 = nn.Linear(400, 120)  # 输入通道数是400，输出通道数是120
        self.fc2 = nn.Linear(120, 84)  # 输入通道数是120，输出通道数是84
        self.fc3 = nn.Linear(84, 10)  # 输入通道数为84，输出通道数是10

    def forward(self, x):
        # batch*1*28*28 -> batch*10*24*24（28x28的图像经过一次核为5x5的卷积，输出变为24x24）
        in_size = x.size(0)
        out = self.conv1(x)
        out = F.relu(out)  # batch*10*24*24（激活函数ReLU不改变形状））
        # batch*10*24*24 -> batch*10*12*12（2*2的池化层会减半）
        out = F.max_pool2d(out, 2, 2)
        out = self.conv2(out)  # batch*10*12*12 -> batch*20*10*10（再卷积一次，核的大小是3）
        out = F.relu(out)  # batch*20*10*10
        out = F.max_pool2d(out, 2, 2)

        out = out.view(out.size(0), -1)
        out = self.fc1(out)  # batch*400 -> batch*120
        out = F.relu(out)  # batch*120
        out = self.fc2(out)  # batch*120 -> batch*84
        out = F.relu(out)
        out = self.fc3(out)
        return out
