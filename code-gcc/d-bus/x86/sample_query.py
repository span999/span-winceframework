#!/usr/bin/python
#
#
#

import dbus

print ("This a test program +++")

bus = dbus.SystemBus()
#bus = dbus.SessionBus()
print ("This a test program 1")
helloservice = bus.get_object('test.method.server', '/test/method/Object')
print ("This a test program 2")
hello = helloservice.get_dbus_method('Method', 'test.method.Type')
print ("This a test program 3")
hello("This is from python!")
print ("This a test program ---")
