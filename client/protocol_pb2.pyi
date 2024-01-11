from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class Request(_message.Message):
    __slots__ = ["id", "req"]
    ID_FIELD_NUMBER: _ClassVar[int]
    REQ_FIELD_NUMBER: _ClassVar[int]
    id: str
    req: int
    def __init__(self, id: _Optional[str] = ..., req: _Optional[int] = ...) -> None: ...

class Response(_message.Message):
    __slots__ = ["id", "res"]
    ID_FIELD_NUMBER: _ClassVar[int]
    RES_FIELD_NUMBER: _ClassVar[int]
    id: str
    res: int
    def __init__(self, id: _Optional[str] = ..., res: _Optional[int] = ...) -> None: ...
