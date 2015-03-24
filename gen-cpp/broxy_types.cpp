/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "broxy_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>

namespace broxy {


Response::~Response() throw() {
}


void Response::__set_code(const int32_t val) {
  this->code = val;
}

void Response::__set_body(const std::string& val) {
  this->body = val;
}

const char* Response::ascii_fingerprint = "3F5FC93B338687BC7235B1AB103F47B3";
const uint8_t Response::binary_fingerprint[16] = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

uint32_t Response::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->code);
          this->__isset.code = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readBinary(this->body);
          this->__isset.body = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Response::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Response");

  xfer += oprot->writeFieldBegin("code", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->code);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("body", ::apache::thrift::protocol::T_STRING, 2);
  xfer += oprot->writeBinary(this->body);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Response &a, Response &b) {
  using ::std::swap;
  swap(a.code, b.code);
  swap(a.body, b.body);
  swap(a.__isset, b.__isset);
}

Response::Response(const Response& other0) {
  code = other0.code;
  body = other0.body;
  __isset = other0.__isset;
}
Response& Response::operator=(const Response& other1) {
  code = other1.code;
  body = other1.body;
  __isset = other1.__isset;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const Response& obj) {
  using apache::thrift::to_string;
  out << "Response(";
  out << "code=" << to_string(obj.code);
  out << ", " << "body=" << to_string(obj.body);
  out << ")";
  return out;
}


Request::~Request() throw() {
}


void Request::__set_url(const std::string& val) {
  this->url = val;
}

const char* Request::ascii_fingerprint = "EFB929595D312AC8F305D5A794CFEDA1";
const uint8_t Request::binary_fingerprint[16] = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

uint32_t Request::read(::apache::thrift::protocol::TProtocol* iprot) {

  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;


  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->url);
          this->__isset.url = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  return xfer;
}

uint32_t Request::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  oprot->incrementRecursionDepth();
  xfer += oprot->writeStructBegin("Request");

  xfer += oprot->writeFieldBegin("url", ::apache::thrift::protocol::T_STRING, 1);
  xfer += oprot->writeString(this->url);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  oprot->decrementRecursionDepth();
  return xfer;
}

void swap(Request &a, Request &b) {
  using ::std::swap;
  swap(a.url, b.url);
  swap(a.__isset, b.__isset);
}

Request::Request(const Request& other2) {
  url = other2.url;
  __isset = other2.__isset;
}
Request& Request::operator=(const Request& other3) {
  url = other3.url;
  __isset = other3.__isset;
  return *this;
}
std::ostream& operator<<(std::ostream& out, const Request& obj) {
  using apache::thrift::to_string;
  out << "Request(";
  out << "url=" << to_string(obj.url);
  out << ")";
  return out;
}

} // namespace
