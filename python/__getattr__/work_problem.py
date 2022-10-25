# 首先，Algo提供给使用者的接口应该尽量简单，因此应该使用algo.func,
# 而不是algo.cur_imp.func。其次，AlgoImpA和AlgoImpB都有很多的属性
#（泛指函数和数据属性），使用__getattr__能大幅简化代码。Why we use python，life is short。
class AlgoImpA(object):
    def __init__(self):
        self.obj_attr = 'obj_attr in AlgoImpA'

    def foo(self):
        print('foo in AlgoImpA')

    def bar(self):
        print('bar in AlgoImpA')

class AlgoImpB(object):
    def __init__(self):
        self.obj_attr = 'obj_attr in AlgoImpB'

    def foo(self):
        print('foo in AlgoImpB')

    def bar(self):
        print('bar in AlgoImpB')

class Algo(object):
    def __init__(self):
        self.imp_a = AlgoImpA()
        self.imp_b = AlgoImpB()
        self.cur_imp = self.imp_a

    def switch_imp(self):
        if self.cur_imp == self.imp_a:
            self.cur_imp = self.imp_b
        else:
            self.cur_imp = self.imp_a

    def __str__(self):
        return 'Algo with imp %s' % str(self.cur_imp)


    def __getattr__(self, name):
        return getattr(self.cur_imp, name)


if __name__ == '__main__':
    algo = Algo()

    print(algo)
    print(algo.obj_attr)
    algo.foo()

    algo.switch_imp()

    print(algo)
    print(algo.obj_attr)
    algo.bar()
