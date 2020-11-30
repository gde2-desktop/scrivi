# -*- coding: utf-8 -*-

# config.py -- Config dialog
#
# Copyright (C) 2008 - B. Clausius
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

# Parts from "Interactive Python-GTK Console" (stolen from epiphany's console.py)
#     Copyright (C), 1998 James Henstridge <james@daa.com.au>
#     Copyright (C), 2005 Adam Hooper <adamh@densi.com>
# Bits from scrivi Python Console Plugin
#     Copyrignt (C), 2005 Raphaël Slinckx

import os
import gtk

__all__ = ('PythonConsoleConfig', 'PythonConsoleConfigDialog')

GDE2CONF_KEY_BASE = '/apps/scrivi/plugins/pythonconsole'
GDE2CONF_KEY_COMMAND_COLOR = GDE2CONF_KEY_BASE + '/command-color'
GDE2CONF_KEY_ERROR_COLOR = GDE2CONF_KEY_BASE + '/error-color'

DEFAULT_COMMAND_COLOR = '#314e6c' # Blue Shadow
DEFAULT_ERROR_COLOR = '#990000' # Accent Red Dark

class PythonConsoleConfig(object):
    try:
        import gde2conf
    except ImportError:
        gde2conf = None

    def __init__(self):
        pass

    @staticmethod
    def enabled():
        return PythonConsoleConfig.gde2conf != None

    @staticmethod
    def add_handler(handler):
        if PythonConsoleConfig.gde2conf:
            PythonConsoleConfig.gde2conf.client_get_default().notify_add(GDE2CONF_KEY_BASE, handler)

    color_command = property(
        lambda self: self.gde2conf_get_str(GDE2CONF_KEY_COMMAND_COLOR, DEFAULT_COMMAND_COLOR),
        lambda self, value: self.gde2conf_set_str(GDE2CONF_KEY_COMMAND_COLOR, value))

    color_error = property(
        lambda self: self.gde2conf_get_str(GDE2CONF_KEY_ERROR_COLOR, DEFAULT_ERROR_COLOR),
        lambda self, value: self.gde2conf_set_str(GDE2CONF_KEY_ERROR_COLOR, value))

    @staticmethod
    def gde2conf_get_str(key, default=''):
        if not PythonConsoleConfig.gde2conf:
            return default

        val = PythonConsoleConfig.gde2conf.client_get_default().get(key)
        if val is not None and val.type == gde2conf.VALUE_STRING:
            return val.get_string()
        else:
            return default

    @staticmethod
    def gde2conf_set_str(key, value):
        if not PythonConsoleConfig.gde2conf:
            return

        v = PythonConsoleConfig.gde2conf.Value(gde2conf.VALUE_STRING)
        v.set_string(value)
        PythonConsoleConfig.gde2conf.client_get_default().set(key, v)

class PythonConsoleConfigDialog(object):

    def __init__(self, datadir):
        object.__init__(self)
        self._dialog = None
        self._ui_path = os.path.join(datadir, 'ui', 'config.ui')
        self.config = PythonConsoleConfig()

    def dialog(self):
        if self._dialog is None:
            self._ui = gtk.Builder()
            self._ui.add_from_file(self._ui_path)

            self.set_colorbutton_color(self._ui.get_object('colorbutton-command'),
                                        self.config.color_command)
            self.set_colorbutton_color(self._ui.get_object('colorbutton-error'),
                                        self.config.color_error)

            self._ui.connect_signals(self)

            self._dialog = self._ui.get_object('dialog-config')
            self._dialog.show_all()
        else:
            self._dialog.present()

        return self._dialog

    @staticmethod
    def set_colorbutton_color(colorbutton, value):
        try:
            color = gtk.gdk.color_parse(value)
        except ValueError:
            pass    # Default color in config.ui used
        else:
            colorbutton.set_color(color)

    def on_dialog_config_response(self, dialog, response_id):
        self._dialog.destroy()

    def on_dialog_config_destroy(self, dialog):
        self._dialog = None
        self._ui = None

    def on_colorbutton_command_color_set(self, colorbutton):
        self.config.color_command = colorbutton.get_color().to_string()

    def on_colorbutton_error_color_set(self, colorbutton):
        self.config.color_error = colorbutton.get_color().to_string()

# ex:et:ts=4:
