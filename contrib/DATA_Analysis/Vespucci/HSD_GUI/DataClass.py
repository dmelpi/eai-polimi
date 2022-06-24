class DataClass(object):
    def __init__(self, comp_name, data):
        self.comp_name = comp_name
        self.data = data

class RawDataClass(object):
    def __init__(self, p_id, ssd, sss, data):
        self.p_id = p_id
        self.sss = sss
        self.ssd = ssd
        self.data = data

class PlotParams(object):
    def __init__(self, comp_name, enabled, odr, dimension) -> None:
        self.comp_name = comp_name
        self.enabled = enabled
        self.odr = odr
        self.dimension = dimension