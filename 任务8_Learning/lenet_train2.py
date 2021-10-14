import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torchvision import datasets, transforms
from torch.autograd import Variable
torch.__version__


BATCH_SIZE = 256  
EPOCHS = 40
DEVICE = torch.device("cpu")


# 下载训练集
train_loader = torch.utils.data.DataLoader(
    datasets.MNIST('data2', train=True, download=False,
                   transform=transforms.Compose([
                       transforms.ToTensor(),
                       transforms.Normalize((0.1307,), (0.3081,))
                   ])),
    batch_size=BATCH_SIZE, shuffle=True)

# 下载测试集
test_loader = torch.utils.data.DataLoader(
    datasets.MNIST('data2', train=False, transform=transforms.Compose([
        transforms.ToTensor(),
        transforms.Normalize((0.1307,), (0.3081,))
    ])),
    batch_size=BATCH_SIZE, shuffle=True)


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


# 训练
def train(model, device, train_loader, optimizer, epoch):
    model.train()
    for batch_idx, (data, target) in enumerate(train_loader):
        data = data.to(device)
        target = target.to(device)
        data, target = Variable(data), Variable(target)
        optimizer.zero_grad()
        output = model(data)
        loss = F.cross_entropy(output, target)
        loss.backward()
        optimizer.step()
        if(batch_idx+1) % 30 == 0:
            print('Train Epoch: {} [{}/{} ({:.0f}%)]\tLoss: {:.6f}'.format(
                epoch, batch_idx * len(data), len(train_loader.dataset),
                100. * batch_idx / len(train_loader), loss.item()))

# 测试


def test(model, device, test_loader):
    model.eval()
    test_loss = 0
    correct = 0
    with torch.no_grad():
        for data, target in test_loader:
            data, target = data.to(device), target.to(device)
            output = model(data)
            test_loss += F.cross_entropy(output, target,
                                    reduction='sum').item()  # 将一批的损失相加
            pred = output.max(1, keepdim=True)[1]  # 找到概率最大的下标
            correct += pred.eq(target.view_as(pred)).sum().item()

    test_loss /= len(test_loader.dataset)
    print('\nTest set: Average loss: {:.4f}, Accuracy: {}/{} ({:.0f}%)\n'.format(
        test_loss, correct, len(test_loader.dataset),
        100. * correct / len(test_loader.dataset)))


if __name__ == '__main__':
    model = ConvNet().to(DEVICE)
    optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.5)

    for epoch in range(1, EPOCHS + 1):
        train(model, DEVICE, train_loader, optimizer, epoch)
        test(model, DEVICE, test_loader)


# 保存训练完成后的模型
torch.save(model, './MNIST2.pth')
