#!/usr/bin/env python
#
# please visit site below for installation
# http://wiki.wxpython.org/How%20to%20install%20wxPython
#

#import os
#import subprocess
#import ftplib
import wx



print "hello!!"
print wx.version()


class sp_MyFrame(wx.Frame):
	""" override the default Frame class """
	def __init__(self, parent, title):
		wx.Frame.__init__(self, parent, title=title, size=(800,400))
		self.control = wx.TextCtrl(self, style=wx.TE_MULTILINE)
		self.InitUI()
        
	def InitUI(self):
		# a statusbar in the bottom of the window
		self.CreateStatusBar()
		self.SetStatusText("Welcome to My Frame...")
		# a toolbar in the top of the window
		self.CreateToolBar()
		# setting up the menu
		filemenu = wx.Menu()
		filemenu.Append(wx.ID_ABOUT, "&About", " The about information")
		filemenu.AppendSeparator()	
		fitemquit = filemenu.Append(wx.ID_EXIT, "E&xit", " End program")
		# creating the menubar
		menuBar = wx.MenuBar()
		menuBar.Append(filemenu, "&File")
		self.SetMenuBar(menuBar)

		self.Bind(wx.EVT_MENU, self.OnQuit, fitemquit)

		# show window
		self.Show(True)

	def OnQuit(self, e):
		self.Close()




app = wx.App(False)  # Create a new app, don't redirect stdout/stderr to a window.
###frame = wx.Frame(None, wx.ID_ANY, "Hello World") # A Frame is a top-level window.
frame = sp_MyFrame(None, 'Hello World !!') # A Frame is a top-level window.
#frame.Show(True)     # Show the frame.
app.MainLoop()





