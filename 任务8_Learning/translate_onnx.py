import torch
from net import ConvNet
import torch.onnx

device = torch.device("cpu")

model = torch.load('./MNIST2.pth')
batch_size = 1
input_shape = (1, 28, 28)

x = torch.randn(1, 1, 28, 28)
x = x.to(device)
export_onnx_file = "MNIST2.onnx"
torch.onnx.export(model, x, export_onnx_file, opset_version=10, do_constant_folding=True, input_names=[
                  "input"], output_names=["output"], dynamic_axes={"input": {0: "batch_size"}, "output": {0: "batch_size"}})
