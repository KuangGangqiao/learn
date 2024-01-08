# 单例是一种创建型设计模式， 让你能够保证一个类只有一个实例， 并提供一个访问该实例的全局节点。
# 单例拥有与全局变量相同的优缺点。 尽管它们非常有用， 但却会破坏代码的模块化特性。
# 在某些其他上下文中， 你不能使用依赖于单例的类。 你也将必须使用单例类。
# 绝大多数情况下， 该限制会在创建单元测试时出现。


# 使用示例： 许多开发者将单例模式视为一种反模式。 因此它在 Python 代码中的使用频率正在逐步减少。
# 识别方法： 单例可以通过返回相同缓存对象的静态构建方法来识别。

# 实现一个粗糙的单例非常简单。 你仅需隐藏构造函数并实现一个静态的构建方法即可。
# 相同的类在多线程环境中会出错。 多线程可能会同时调用构建方法并获取多个单例类的实例。

class SingletonMeta(type):
    """
    The Singleton class can be implemented in different ways in Python. Some
    possible methods include: base class, decorator, metaclass. We will use the
    metaclass because it is best suited for this purpose.
    """

    _instances = {}

    def __call__(cls, *args, **kwargs):
        """
        Possible changes to the value of the `__init__` argument do not affect
        the returned instance.
        """
        if cls not in cls._instances:
            instance = super().__call__(*args, **kwargs)
            cls._instances[cls] = instance
        return cls._instances[cls]


class Singleton(metaclass=SingletonMeta):
    def some_business_logic(self):
        """
        Finally, any singleton should define some business logic, which can be
        executed on its instance.
        """

        # ...


if __name__ == "__main__":
    # The client code.

    s1 = Singleton()
    s2 = Singleton()

    if id(s1) == id(s2):
        print("Singleton works, both variables contain the same instance.")
    else:
        print("Singleton failed, variables contain different instances.")
