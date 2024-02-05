# Class to parse the configuration file of the SoC, allow user to choose which vulnerabilites to use.
# The instance of the configuration can be imported and share between every scripts.

import configparser

class Config:
    __instance = None

    @staticmethod 
    def getInstance():
        if Config.__instance == None:
            Config()
        return Config.__instance

    def __init__(self):
        if Config.__instance != None:
            raise Exception("An instance of the class is already created !")
        else:
            Config.__instance = self

    def readFile(self, file_path):
        self.config = configparser.ConfigParser()
        self.config.read(file_path)

    def getValue(self, key):
        if not hasattr(self, "config"):
            return False
        return self.config.getboolean('VULNERABILITIES', key)
    
    def getInt(self, key):
        if not hasattr(self, "config"):
            return False
        return self.config.getint('VULNERABILITIES', key)
    
    def getString(self, key):
        if not hasattr(self, "config"):
            return False
        return self.config.get('VULNERABILITIES', key)

    def setValue(self, key, value):
        self.config['VULNERABILITIES'][key] = value