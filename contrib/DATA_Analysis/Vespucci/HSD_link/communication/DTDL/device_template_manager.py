import json
from HSD_link.communication.DTDL import device_template_model as DTM

class DeviceTemplateManager:

    def __init__(self, device_template_json: str) -> None:#, fw_name: str) -> None:
        with open(device_template_json, 'r', encoding='utf-8') as json_file:
            self.device_template_model = json.load(json_file)
            # self.fw_name = fw_name
        #TODO raise exception (Invalid Device Template) manage invalid templates
        # return -1
    
    # def get_interface_name_from_id(id: str):
    #     return id.split(":")[-1].split(";")[0]

    def __get_interface_list(self):
        interfaces = []
        for d in self.device_template_model:
            interfaces.append(DTM.InterfaceElement.from_dict(d)) #TODO if d contains request!!! 
        return interfaces
    
    def __is_root_interface(self, interface):
        return len(interface.id.split(":")) == 4

    def get_root_component(self):
        for interface in self.__get_interface_list():
            if self.__is_root_interface(interface):
                return interface
            # interface_name = DeviceTemplateManager.get_interface_name_from_id(interface.id)
            # if(interface_name == self.fw_name):
            #     return interface
    
    def get_components(self):    
        component_dict = {}
        for content in self.get_root_component().contents:
            if content.type == DTM.ContentType.COMPONENT:
                for interface in self.__get_interface_list():
                    if(content.schema == interface.id):
                        component_dict[content.name] = interface
        return component_dict

    def get_components_name_list(self):
        comp_names_list = []
        for c in self.get_components():
            comp_names_list.append(c.name)
        return comp_names_list

    def get_component(self, comp_name:str):
        try:
            res = self.get_components()[comp_name]
            return res
        except:
            print("[ERROR] - Component \'{}\' doesn't exist in your selected Device Template".format(comp_name))
