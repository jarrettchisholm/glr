#
# Scons Output Colorizer v0.2
# Copyright (C) 2008, 2009 Mihail 'IceBreaker' Szabolcs 
#
# e-mail: theicebreaker007@gmail.com
# web: http://www.szabster.net
#
# Inspired by Blender's excellent SConstruct.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

import os
import SCons

isWindows = os.name == 'nt'

# shortcuts
Action = SCons.Action.Action
Builder = SCons.Builder.Builder
Tool = SCons.Tool

class colorizer:
	# define various colors		
	cPurple	= '\033[95m'
	cBlue 	= '\033[94m'
	cGreen 	= '\033[92m'
	cYellow = '\033[93m'
	cRed 	= '\033[91m'
	cEnd 	= '\033[0m'
	
	# standardized color message class
	class message:
		# stores message and creates Scons action
		def __init__(self, action, text, c1, c2, c3, end, target = False):
			self.text 	= text
			self.c1 	= c1
			self.c2 	= c2
			self.c3 	= c3
			self.end	= end
			self.target	= target
			self.action = Action(str(action),strfunction=self.strfunction)
			return
		# get the ready prepapred Scons Action
		def getAction(self):
			return self.action;
		# message output used by the Scons Action
		def strfunction(self, target, source, env):
			if self.target:
				a = '%s' % (target[0])
			else:
				a = '%s' % (source[0])
				
			d, f = os.path.split(a)
			e = self.end
			t = str(self.text)
			return self.c1+t+e+self.c2+" ==> "+e+"'"+self.c3+"%s" % (f) +e+"'"+e

	def __init__(self,noColors=False):

		if isWindows:
			noColors = True

		# just customize output, no colors
		if noColors:
			self.disableColors()
		
		# C compile messages
		self.cmpCStaticMsg = self.message("$CCCOM","Compiling",
											self.cBlue,self.cPurple,self.cYellow,self.cEnd)
		self.cmpCSharedMsg = self.message("$SHCCCOM","Compiling",
											self.cBlue,self.cPurple,self.cYellow,self.cEnd)
											
		# C++ compile messages
		self.cmpCPPStaticMsg = self.message("$CXXCOM","Compiling",
											self.cBlue,self.cPurple,self.cYellow,self.cEnd)
		self.cmpCPPSharedMsg = self.message("$SHCXXCOM","Compiling",
											self.cBlue,self.cPurple,self.cYellow,self.cEnd)
											
		# Linking messages
		self.lnkStaticMsg = self.message("$ARCOM","Linking library",
											self.cRed,self.cPurple,self.cYellow,self.cEnd,True)
		self.lnkSharedMsg = self.message("$SHLINKCOM","Linking shared object",
											self.cRed,self.cPurple,self.cYellow,self.cEnd,True)
		self.lnkProgramMsg= self.message("$LINKCOM","Linking program",
											self.cRed,self.cPurple,self.cYellow,self.cEnd,True)

		return

	# set colors to empty strings thus disabling them on output
	def disableColors(self):
		self.cPurple	= ''
		self.cBlue 		= ''
		self.cGreen 	= ''
		self.cYellow	= ''
		self.cRed		= ''
		self.cEnd 		= ''
		return

	# colorize any given Scons Environment
	def colorize(self, env):
		if not env:
			return False
		
		# customize linking ...
		self.colorizeBuilder(env,'Program');
		self.colorizeBuilder(env,'Library');
		self.colorizeBuilder(env,'StaticLibrary');
		self.colorizeBuilder(env,'SharedLibrary');
		self.colorizeBuilder(env,'LoadableModule');
		
		# customize compiling ...
		self.colorizeObjBuilders(env)
		
		return True		
		
	# colorize the object builders from any given Scons Environment
	def colorizeObjBuilders(self, env):
		if not env:
			return False
			
		# Object Builders
		static_ob, shared_ob = Tool.createObjBuilders(env)

		# Set Static Object actions
		static_ob.add_action('.c'	, self.cmpCStaticMsg.getAction() 	)
		static_ob.add_action('.cpp'	, self.cmpCPPStaticMsg.getAction() 	)
		# Set Shared Object actions
		shared_ob.add_action('.c'	, self.cmpCSharedMsg.getAction()	)
		shared_ob.add_action('.cpp'	, self.cmpCPPSharedMsg.getAction()	)
		
		return True
		
	# colorize any given builder from any given Scons Environment
	def colorizeBuilder(self, env, builder, text="", target = True, color=""):
		if not env or not builder:
			return False
			
		# set default color to red
		if not color:
			color = self.cRed

		action = ""
		
		# detect and use pre-built actions
		if builder == 'SharedLibrary' or builder == 'LoadableModule':
			action = self.lnkSharedMsg.getAction()
		elif builder == 'StaticLibrary' or builder == 'Library':
			action = self.lnkStaticMsg.getAction()
		elif builder == 'Program':
			action = self.lnkProgramMsg.getAction()
		elif text:	# add customized output to any given builder
			_action = env['BUILDERS'][builder].action
			action = self.message(_action,text,color,self.cPurple,self.cYellow,self.cEnd,target).getAction()
			
		# 1,2,3 action!	
		if action:
			try:
				env['BUILDERS'][builder].action = action
			except:
				return False
				
		return True
