# 适配器是一种结构型设计模式， 它能使不兼容的对象能够相互合作。
# 适配器可担任两个对象间的封装器， 它会接收对于一个对象的调用，
# 并将其转换为另一个对象可识别的格式和接口。
# 使用示例： 适配器模式在 Python 代码中很常见。 基于一些遗留代码的系统常常会使用该模式。
# 在这种情况下， 适配器让遗留代码与现代的类得以相互合作。
# 识别方法： 适配器可以通过以不同抽象或接口类型实例为参数的构造函数来识别。
# 当适配器的任何方法被调用时， 它会将参数转换为合适的格式， 然后将调用定向到其封装对象中的一个或多个方法。
# 概念示例
#
# 本例说明了适配器设计模式的结构并重点回答了下面的问题：
#
#    它由哪些类组成？
#    这些类扮演了哪些角色？
#    模式中的各个元素会以何种方式相互关联？

class Target:
    """
    The Target defines the domain-specific interface used by the client code.
    """

    def request(self) -> str:
        return "Target: The default target's behavior."


class Adaptee:
    """
    The Adaptee contains some useful behavior, but its interface is incompatible
    with the existing client code. The Adaptee needs some adaptation before the
    client code can use it.
    """

    def specific_request(self) -> str:
        return ".eetpadA eht fo roivaheb laicepS"


class Adapter(Target, Adaptee):
    """
    The Adapter makes the Adaptee's interface compatible with the Target's
    interface via multiple inheritance.
    """

    def request(self) -> str:
        return f"Adapter: (TRANSLATED) {self.specific_request()[::-1]}"


def client_code(target: "Target") -> None:
    """
    The client code supports all classes that follow the Target interface.
    """

    print(target.request(), end="")


if __name__ == "__main__":
    print("Client: I can work just fine with the Target objects:")
    target = Target()
    client_code(target)
    print("\n")

    adaptee = Adaptee()
    print("Client: The Adaptee class has a weird interface. "
          "See, I don't understand it:")
    print(f"Adaptee: {adaptee.specific_request()}", end="\n\n")

    print("Client: But I can work with it via the Adapter:")
    adapter = Adapter()
    client_code(adapter)

