import time
from HSD_link.HSDLink_v2 import HSDLink_v2
from HSD_link.HSDLink_v1 import HSDLink_v1
from HSD_utils.exceptions import CommunicationEngineOpenError
import HSD_utils.logger as logger

log = logger.get_logger(__name__)

class HSDLink:
    def create_hsd_link(self, dev_com_type: str = 'stwin_hsd', acquisition_folder = None):
        self.dev_com_type = dev_com_type
        self.acquisition_folder = acquisition_folder
        self.is_datalog2 = False
        try:
            hsd_link = HSDLink_v1(self.dev_com_type, self.acquisition_folder)
        except CommunicationEngineOpenError:
            self.is_datalog2 = True

        time.sleep(2)

        if self.is_datalog2:
            if self.dev_com_type == "stwin_hsd":
                self.dev_com_type = "pnpl"
            hsd_link = HSDLink_v2(self.dev_com_type, self.acquisition_folder)

        return hsd_link