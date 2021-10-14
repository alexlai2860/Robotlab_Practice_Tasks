import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms
torch.__version__

class ConvNet(nn.Module):
    def __init__(self):
        super().__init__()
        #  卷积层
        self.conv1 = nn.Conv2d(1, 6, 5, padding=2)  # 输入通道数1，输出通道数6，核的大小5
        self.conv2 = nn.Conv2d(6, 16, 5)  # 输入通道数6，输出通道数16，核的大小5
        # 全连接层
        self.fc1 = nn.Linear(400, 120)  # 输入通道数是400，输出通道数是120
        self.fc2 = nn.Linear(120, 84)  # 输入通道数是120，输出通道数是84
        self.fc3 = nn.Linear(84, 10)  # 输入通道数为84，输出通道数是10

    def forward(self, x):
        in_size = x.size(0)
        out = self.conv1(x)
        out = F.relu(out)
        out = F.max_pool2d(out, 2, 2)
        out = self.conv2(out)
        out = F.relu(out)
        out = F.max_pool2d(out, 2, 2)

        out = out.view(out.size(0), -1)
        out = self.fc1(out)  # batch*400 -> batch*120
        out = F.relu(out)  # batch*120
        out = self.fc2(out)  # batch*120 -> batch*84
        out = F.relu(out)
        out = self.fc3(out)
        return out