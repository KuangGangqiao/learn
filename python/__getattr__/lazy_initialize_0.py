# 第二个例子，对象属性的lazy initialize。
# 输出：
#     before {}
#     after {'lazy_loaded_attr': 'lazy_loaded_attr'}
#
# 可以看到，属性访问前对象中的__dict__没有任何元素，访问之后就有添加。
# 这个例子是类实例的属性的惰性初始化，bottle里面也有一个用descriptor实现类属性的惰性初始化。

# code
class WidgetShowLazyLoad(object):
    def fetch_complex_attr(self, attrname):
        '''可能是比较耗时的操作， 比如从文件读取'''
        return attrname

    def __getattr__(self, name):
        if name not in self.__dict__:
             self.__dict__[name] = self.fetch_complex_attr(name)
        return self.__dict__[name]

if __name__ == '__main__':
    w = WidgetShowLazyLoad()
    print('before', w.__dict__)
    w.lazy_loaded_attr
    print('after', w.__dict__)
