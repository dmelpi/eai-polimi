# To use this code, make sure you
#
#     import json
#
# and then, to convert JSON from a string, do
#
#     result = welcome_from_dict(json.loads(json_string))


from dataclasses import dataclass
from typing import Any, List, TypeVar, Type, Callable, cast
from enum import Enum

T = TypeVar("T")
EnumT = TypeVar("EnumT", bound=Enum)


def from_str(x: Any) -> str:
    assert isinstance(x, str)
    return x


def from_int(x: Any) -> int:
    assert isinstance(x, int) and not isinstance(x, bool)
    return x


def to_enum(c: Type[EnumT], x: Any) -> EnumT:
    assert isinstance(x, c)
    return x.value


def from_list(f: Callable[[Any], T], x: Any) -> List[T]:
    assert isinstance(x, list)
    return [f(y) for y in x]


def from_float(x: Any) -> float:
    assert isinstance(x, (float, int)) and not isinstance(x, bool)
    return float(x)


def to_float(x: Any) -> float:
    assert isinstance(x, float)
    return x


def to_class(c: Type[T], x: Any) -> dict:
    assert isinstance(x, c)
    return cast(Any, x).to_dict()


def from_bool(x: Any) -> bool:
    assert isinstance(x, bool)
    return x


@dataclass
class DeviceInfo:
    serial_number: str
    alias: str
    part_number: str
    url: str
    fw_name: str
    fw_version: str
    data_file_ext: str
    data_file_format: str
    n_sensor: int

    @staticmethod
    def from_dict(obj: Any) -> 'DeviceInfo':
        assert isinstance(obj, dict)
        serial_number = from_str(obj.get("serialNumber"))
        alias = from_str(obj.get("alias"))
        part_number = from_str(obj.get("partNumber"))
        url = from_str(obj.get("URL"))
        fw_name = from_str(obj.get("fwName"))
        fw_version = from_str(obj.get("fwVersion"))
        data_file_ext = from_str(obj.get("dataFileExt"))
        data_file_format = from_str(obj.get("dataFileFormat"))
        n_sensor = from_int(obj.get("nSensor"))
        return DeviceInfo(serial_number, alias, part_number, url, fw_name, fw_version, data_file_ext, data_file_format,
                          n_sensor)

    def to_dict(self) -> dict:
        result: dict = {}
        result["serialNumber"] = from_str(self.serial_number)
        result["alias"] = from_str(self.alias)
        result["partNumber"] = from_str(self.part_number)
        result["URL"] = from_str(self.url)
        result["fwName"] = from_str(self.fw_name)
        result["fwVersion"] = from_str(self.fw_version)
        result["dataFileExt"] = from_str(self.data_file_ext)
        result["dataFileFormat"] = from_str(self.data_file_format)
        result["nSensor"] = from_int(self.n_sensor)
        return result


class DataType(Enum):
    FLOAT = "float"
    INT16_T = "int16_t"


@dataclass
class SamplesPerTs:
    min: int
    max: int
    data_type: DataType

    @staticmethod
    def from_dict(obj: Any) -> 'SamplesPerTs':
        assert isinstance(obj, dict)
        min = from_int(obj.get("min"))
        max = from_int(obj.get("max"))
        data_type = DataType(obj.get("dataType"))
        return SamplesPerTs(min, max, data_type)

    def to_dict(self) -> dict:
        result: dict = {}
        result["min"] = from_int(self.min)
        result["max"] = from_int(self.max)
        result["dataType"] = to_enum(DataType, self.data_type)
        return result


@dataclass
class SubSensorDescriptor:
    id: int
    sensor_type: str
    dimensions: int
    dimensions_label: List[str]
    unit: str
    data_type: DataType
    fs: List[float]
    odr: List[float]
    samples_per_ts: SamplesPerTs

    @staticmethod
    def from_dict(obj: Any) -> 'SubSensorDescriptor':
        assert isinstance(obj, dict)
        id = from_int(obj.get("id"))
        sensor_type = from_str(obj.get("sensorType"))
        dimensions = from_int(obj.get("dimensions"))
        dimensions_label = from_list(from_str, obj.get("dimensionsLabel"))
        unit = from_str(obj.get("unit"))
        # data_type = DataType(obj.get("dataType"))
        data_type = from_str(obj.get("dataType"))
        fs = from_list(from_float, obj.get("FS"))
        odr = from_list(from_float, obj.get("ODR"))
        samples_per_ts = SamplesPerTs.from_dict(obj.get("samplesPerTs"))
        return SubSensorDescriptor(id, sensor_type, dimensions, dimensions_label, unit, data_type, fs, odr,
                                   samples_per_ts)

    def to_dict(self) -> dict:
        result: dict = {}
        result["id"] = from_int(self.id)
        result["sensorType"] = from_str(self.sensor_type)
        result["dimensions"] = from_int(self.dimensions)
        result["dimensionsLabel"] = from_list(from_str, self.dimensions_label)
        result["unit"] = from_str(self.unit)
        result["dataType"] = to_enum(DataType, self.data_type)
        result["FS"] = from_list(to_float, self.fs)
        result["ODR"] = from_list(to_float, self.odr)
        result["samplesPerTs"] = to_class(SamplesPerTs, self.samples_per_ts)
        return result


@dataclass
class SensorDescriptor:
    sub_sensor_descriptor: List[SubSensorDescriptor]

    @staticmethod
    def from_dict(obj: Any) -> 'SensorDescriptor':
        assert isinstance(obj, dict)
        sub_sensor_descriptor = from_list(SubSensorDescriptor.from_dict, obj.get("subSensorDescriptor"))
        return SensorDescriptor(sub_sensor_descriptor)

    def to_dict(self) -> dict:
        result: dict = {}
        result["subSensorDescriptor"] = from_list(lambda x: to_class(SubSensorDescriptor, x),
                                                  self.sub_sensor_descriptor)
        return result


@dataclass
class SubSensorStatus:
    odr: float
    odr_measured: float
    initial_offset: float
    fs: float
    sensitivity: float
    is_active: bool
    samples_per_ts: int
    usb_data_packet_size: int
    sd_write_buffer_size: int
    wifi_data_packet_size: int
    com_channel_number: int

    @staticmethod
    def from_dict(obj: Any) -> 'SubSensorStatus':
        assert isinstance(obj, dict)
        ord = from_float(obj.get("ODR"))
        ord_measured = from_float(obj.get("ODRMeasured"))
        initial_offset = from_float(obj.get("initialOffset"))
        fs = from_float(obj.get("FS"))
        sensitivity = from_float(obj.get("sensitivity"))
        is_active = from_bool(obj.get("isActive"))
        samples_per_ts = from_int(obj.get("samplesPerTs"))
        usb_data_packet_size = from_int(obj.get("usbDataPacketSize"))
        sd_write_buffer_size = from_int(obj.get("sdWriteBufferSize"))
        wifi_data_packet_size = from_int(obj.get("wifiDataPacketSize"))
        com_channel_number = from_int(obj.get("comChannelNumber"))
        return SubSensorStatus(ord, ord_measured, initial_offset, fs, sensitivity, is_active, samples_per_ts,
                               usb_data_packet_size, sd_write_buffer_size, wifi_data_packet_size, com_channel_number)

    def to_dict(self) -> dict:
        result: dict = {}
        result["ODR"] = to_float(self.odr)
        result["ODRMeasured"] = to_float(self.odr_measured)
        result["initialOffset"] = to_float(self.initial_offset)
        result["FS"] = to_float(self.fs)
        result["sensitivity"] = to_float(self.sensitivity)
        result["isActive"] = from_bool(self.is_active)
        result["samplesPerTs"] = from_int(self.samples_per_ts)
        result["usbDataPacketSize"] = from_int(self.usb_data_packet_size)
        result["sdWriteBufferSize"] = from_int(self.sd_write_buffer_size)
        result["wifiDataPacketSize"] = from_int(self.wifi_data_packet_size)
        result["comChannelNumber"] = from_int(self.com_channel_number)
        return result


@dataclass
class SensorStatus:
    sub_sensor_status: List[SubSensorStatus]

    @staticmethod
    def from_dict(obj: Any) -> 'SensorStatus':
        assert isinstance(obj, dict)
        sub_sensor_status = from_list(SubSensorStatus.from_dict, obj.get("subSensorStatus"))
        return SensorStatus(sub_sensor_status)

    def to_dict(self) -> dict:
        result: dict = {}
        result["subSensorStatus"] = from_list(lambda x: to_class(SubSensorStatus, x), self.sub_sensor_status)
        return result


@dataclass
class Sensor:
    id: int
    name: str
    sensor_descriptor: SensorDescriptor
    sensor_status: SensorStatus

    @staticmethod
    def from_dict(obj: Any) -> 'Sensor':
        assert isinstance(obj, dict)
        id = from_int(obj.get("id"))
        name = from_str(obj.get("name"))
        sensor_descriptor = SensorDescriptor.from_dict(obj.get("sensorDescriptor"))
        sensor_status = SensorStatus.from_dict(obj.get("sensorStatus"))
        return Sensor(id, name, sensor_descriptor, sensor_status)

    def to_dict(self) -> dict:
        result: dict = {}
        result["id"] = from_int(self.id)
        result["name"] = from_str(self.name)
        result["sensorDescriptor"] = to_class(SensorDescriptor, self.sensor_descriptor)
        result["sensorStatus"] = to_class(SensorStatus, self.sensor_status)
        return result


@dataclass
class HwTag:
    id: int
    pin_desc: str
    label: str
    enabled: bool

    @staticmethod
    def from_dict(obj: Any) -> 'HwTag':
        assert isinstance(obj, dict)
        id = from_int(obj.get("id"))
        pin_desc = from_str(obj.get("pinDesc"))
        label = from_str(obj.get("label"))
        enabled = from_bool(obj.get("enabled"))
        return HwTag(id, pin_desc, label, enabled)

    def to_dict(self) -> dict:
        result: dict = {}
        result["id"] = from_int(self.id)
        result["pinDesc"] = from_str(self.pin_desc)
        result["label"] = from_str(self.label)
        result["enabled"] = from_bool(self.enabled)
        return result


@dataclass
class SwTag:
    id: int
    label: str

    @staticmethod
    def from_dict(obj: Any) -> 'SwTag':
        assert isinstance(obj, dict)
        id = from_int(obj.get("id"))
        label = from_str(obj.get("label"))
        return SwTag(id, label)

    def to_dict(self) -> dict:
        result: dict = {}
        result["id"] = from_int(self.id)
        result["label"] = from_str(self.label)
        return result


@dataclass
class TagConfig:
    max_tags_per_acq: int
    sw_tags: List[SwTag]
    hw_tags: List[HwTag]

    @staticmethod
    def from_dict(obj: Any) -> 'TagConfig':
        assert isinstance(obj, dict)
        max_tags_per_acq = from_int(obj.get("maxTagsPerAcq"))
        sw_tags = from_list(SwTag.from_dict, obj.get("swTags"))
        hw_tags = from_list(HwTag.from_dict, obj.get("hwTags"))
        return TagConfig(max_tags_per_acq, sw_tags, hw_tags)

    def to_dict(self) -> dict:
        result: dict = {}
        result["maxTagsPerAcq"] = from_int(self.max_tags_per_acq)
        result["swTags"] = from_list(lambda x: to_class(SwTag, x), self.sw_tags)
        result["hwTags"] = from_list(lambda x: to_class(HwTag, x), self.hw_tags)
        return result


@dataclass
class Device:
    device_info: DeviceInfo
    sensor: List[Sensor]
    tag_config: TagConfig

    @staticmethod
    def from_dict(obj: Any) -> 'Device':
        assert isinstance(obj, dict)
        device_info = DeviceInfo.from_dict(obj.get("deviceInfo"))
        sensor = from_list(Sensor.from_dict, obj.get("sensor"))
        tag_config = TagConfig.from_dict(obj.get("tagConfig"))
        return Device(device_info, sensor, tag_config)

    def to_dict(self) -> dict:
        result: dict = {}
        result["deviceInfo"] = to_class(DeviceInfo, self.device_info)
        result["sensor"] = from_list(lambda x: to_class(Sensor, x), self.sensor)
        result["tagConfig"] = to_class(TagConfig, self.tag_config)
        return result


@dataclass
class Acquisition:
    uuid_acquisition: str
    json_version: str
    device: Device

    @staticmethod
    def from_dict(obj: Any) -> 'Acquisition':
        assert isinstance(obj, dict)
        uuid_acquisition = from_str(obj.get("UUIDAcquisition"))
        json_version = from_str(obj.get("JSONVersion"))
        device = Device.from_dict(obj.get("device"))
        return Acquisition(uuid_acquisition, json_version, device)

    def to_dict(self) -> dict:
        result: dict = {}
        result["UUIDAcquisition"] = from_str(self.uuid_acquisition)
        result["JSONVersion"] = from_str(self.json_version)
        result["device"] = to_class(Device, self.device)
        return result


def welcome_from_dict(s: Any) -> Acquisition:
    return Acquisition.from_dict(s)


def welcome_to_dict(x: Acquisition) -> Any:
    return to_class(Acquisition, x)
