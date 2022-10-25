# 第三个例子，我觉的是最实用的，__getattr__使得实现adapter wrapper模式非常容易，
# 我们都知道“组合优于继承”，__getattr__实现的adapter就是以组合的形式。
# 如果adapter需要修改adaptee的行为，那么定义一个同名的属性就行了，
# 其他的想直接“继承”的属性，通通交给__getattr__就行了

class adaptee(object):
    def foo(self):
        print 'foo in adaptee'
    def bar(self):
        print 'bar in adaptee'

class adapter(object):
    def __init__(self):
        self.adaptee = adaptee()

    def foo(self):
        print 'foo in adapter'
        self.adaptee.foo()

    def __getattr__(self, name):
        return getattr(self.adaptee, name)

if __name__ == '__main__':
    a = adapter()
    a.foo()
    a.bar()
