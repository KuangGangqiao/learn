# MetaClass元类，本质也是一个类，但和普通类的用法不同，它可以对类内部的定义
#（包括类属性和类方法）进行动态的修改。可以这么说，使用元类的主要目的就是为了实现在创建类时，
# 能够动态地改变类中定义的属性或者方法。
#
# 不要从字面上去理解元类的含义，事实上 MetaClass 中的 Meta 这个词根，
# 起源于希腊语词汇 meta，包含“超越”和“改变”的意思。
#
# 举个例子，根据实际场景的需要，我们要为多个类添加一个 name 属性和一个 say() 方法。
# 显然有多种方法可以实现，但其中一种方法就是使用 MetaClass 元类。
#
# 如果在创建类时，想用 MetaClass 元类动态地修改内部的属性或者方法，
# 则类的创建过程将变得复杂：先创建 MetaClass 元类，然后用元类去创建类，
# 最后使用该类的实例化对象实现功能。
#
#和前面章节创建的类不同，如果想把一个类设计成 MetaClass 元类，其必须符合以下条件：
#
# 1.必须显式继承自 type 类；
# 2.类中需要定义并实现 __new__() 方法，该方法一定要返回该类的一个实例对象，
# 因为在使用元类创建类时，该 __new__() 方法会自动被执行，用来修改新建的类。
#
# 我们先尝试定义一个 MetaClass 元类：

# 定义一个元类
class FirstMetaClass(type):
    # cls代表动态修改的类
    # name代表动态修改的类名
    # bases代表被动态修改的类的所有父类
    # attr代表被动态修改的类的所有属性、方法组成的字典
    def __new__(cls, name, bases, attrs):
        # 动态为该类添加一个name属性
        attrs['name'] = "jojo"
        attrs['say'] = lambda self: print("调用 say() 实例方法")
        return super().__new__(cls,name,bases,attrs)

# 此程序中，首先可以断定 FirstMetaClass 是一个类。其次，由于该类继承自 type 类，
# 并且内部实现了 __new__() 方法，因此可以断定 FirstMetaCLass 是一个元类

# 可以看到，在这个元类的 __new__() 方法中，手动添加了一个 name 属性和 say() 方法。
# 这意味着，通过 FirstMetaClass 元类创建的类，会额外添加 name 属性和 say() 方法。
# 通过如下代码，可以验证这个结论：

#定义类时，指定元类
class CLanguage(object,metaclass=FirstMetaClass):
    pass

if __name__=='__main__':
    clangs = CLanguage()
    print(clangs.name)
    clangs.say()

# 显然，FirstMetaClass 元类的 __new__() 方法动态地为 Clanguage
# 类添加了 name 属性和 say() 方法，
# 因此，即便该类在定义时是空类，它也依然有 name 属性和 say() 方法。

#对于 MetaClass 元类，它多用于创建 API，因此我们几乎不会使用到它。
