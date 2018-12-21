// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: update_engine.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "update_engine.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace chromeos_update_engine {

void protobuf_ShutdownFile_update_5fengine_2eproto() {
  delete DlcInfo::default_instance_;
  delete DlcParameters::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_update_5fengine_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_update_5fengine_2eproto() GOOGLE_ATTRIBUTE_COLD;
void protobuf_AddDesc_update_5fengine_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  DlcInfo::default_instance_ = new DlcInfo();
  DlcParameters::default_instance_ = new DlcParameters();
  DlcInfo::default_instance_->InitAsDefaultInstance();
  DlcParameters::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_update_5fengine_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_update_5fengine_2eproto_once_);
void protobuf_AddDesc_update_5fengine_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_update_5fengine_2eproto_once_,
                 &protobuf_AddDesc_update_5fengine_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_update_5fengine_2eproto {
  StaticDescriptorInitializer_update_5fengine_2eproto() {
    protobuf_AddDesc_update_5fengine_2eproto();
  }
} static_descriptor_initializer_update_5fengine_2eproto_;
#endif

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DlcInfo::kDlcIdFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DlcInfo::DlcInfo()
  : ::google::protobuf::MessageLite(), _arena_ptr_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:chromeos_update_engine.DlcInfo)
}

void DlcInfo::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

DlcInfo::DlcInfo(const DlcInfo& from)
  : ::google::protobuf::MessageLite(),
    _arena_ptr_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:chromeos_update_engine.DlcInfo)
}

void DlcInfo::SharedCtor() {
    _is_default_instance_ = false;
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  dlc_id_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

DlcInfo::~DlcInfo() {
  // @@protoc_insertion_point(destructor:chromeos_update_engine.DlcInfo)
  SharedDtor();
}

void DlcInfo::SharedDtor() {
  dlc_id_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void DlcInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const DlcInfo& DlcInfo::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_update_5fengine_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_update_5fengine_2eproto();
#endif
  return *default_instance_;
}

DlcInfo* DlcInfo::default_instance_ = NULL;

DlcInfo* DlcInfo::New(::google::protobuf::Arena* arena) const {
  DlcInfo* n = new DlcInfo;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void DlcInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:chromeos_update_engine.DlcInfo)
  dlc_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

bool DlcInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:chromeos_update_engine.DlcInfo)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string dlc_id = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_dlc_id()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->dlc_id().data(), this->dlc_id().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "chromeos_update_engine.DlcInfo.dlc_id"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:chromeos_update_engine.DlcInfo)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:chromeos_update_engine.DlcInfo)
  return false;
#undef DO_
}

void DlcInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:chromeos_update_engine.DlcInfo)
  // optional string dlc_id = 1;
  if (this->dlc_id().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->dlc_id().data(), this->dlc_id().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "chromeos_update_engine.DlcInfo.dlc_id");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->dlc_id(), output);
  }

  // @@protoc_insertion_point(serialize_end:chromeos_update_engine.DlcInfo)
}

int DlcInfo::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:chromeos_update_engine.DlcInfo)
  int total_size = 0;

  // optional string dlc_id = 1;
  if (this->dlc_id().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->dlc_id());
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void DlcInfo::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const DlcInfo*>(&from));
}

void DlcInfo::MergeFrom(const DlcInfo& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:chromeos_update_engine.DlcInfo)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  if (from.dlc_id().size() > 0) {

    dlc_id_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.dlc_id_);
  }
}

void DlcInfo::CopyFrom(const DlcInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:chromeos_update_engine.DlcInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DlcInfo::IsInitialized() const {

  return true;
}

void DlcInfo::Swap(DlcInfo* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DlcInfo::InternalSwap(DlcInfo* other) {
  dlc_id_.Swap(&other->dlc_id_);
  _unknown_fields_.Swap(&other->_unknown_fields_);
  std::swap(_cached_size_, other->_cached_size_);
}

::std::string DlcInfo::GetTypeName() const {
  return "chromeos_update_engine.DlcInfo";
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// DlcInfo

// optional string dlc_id = 1;
void DlcInfo::clear_dlc_id() {
  dlc_id_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 const ::std::string& DlcInfo::dlc_id() const {
  // @@protoc_insertion_point(field_get:chromeos_update_engine.DlcInfo.dlc_id)
  return dlc_id_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void DlcInfo::set_dlc_id(const ::std::string& value) {
  
  dlc_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:chromeos_update_engine.DlcInfo.dlc_id)
}
 void DlcInfo::set_dlc_id(const char* value) {
  
  dlc_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:chromeos_update_engine.DlcInfo.dlc_id)
}
 void DlcInfo::set_dlc_id(const char* value, size_t size) {
  
  dlc_id_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:chromeos_update_engine.DlcInfo.dlc_id)
}
 ::std::string* DlcInfo::mutable_dlc_id() {
  
  // @@protoc_insertion_point(field_mutable:chromeos_update_engine.DlcInfo.dlc_id)
  return dlc_id_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* DlcInfo::release_dlc_id() {
  // @@protoc_insertion_point(field_release:chromeos_update_engine.DlcInfo.dlc_id)
  
  return dlc_id_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void DlcInfo::set_allocated_dlc_id(::std::string* dlc_id) {
  if (dlc_id != NULL) {
    
  } else {
    
  }
  dlc_id_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), dlc_id);
  // @@protoc_insertion_point(field_set_allocated:chromeos_update_engine.DlcInfo.dlc_id)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// ===================================================================

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int DlcParameters::kDlcInfosFieldNumber;
const int DlcParameters::kOmahaUrlFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

DlcParameters::DlcParameters()
  : ::google::protobuf::MessageLite(), _arena_ptr_(NULL) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:chromeos_update_engine.DlcParameters)
}

void DlcParameters::InitAsDefaultInstance() {
  _is_default_instance_ = true;
}

DlcParameters::DlcParameters(const DlcParameters& from)
  : ::google::protobuf::MessageLite(),
    _arena_ptr_(NULL) {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:chromeos_update_engine.DlcParameters)
}

void DlcParameters::SharedCtor() {
    _is_default_instance_ = false;
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  omaha_url_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

DlcParameters::~DlcParameters() {
  // @@protoc_insertion_point(destructor:chromeos_update_engine.DlcParameters)
  SharedDtor();
}

void DlcParameters::SharedDtor() {
  omaha_url_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void DlcParameters::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const DlcParameters& DlcParameters::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_update_5fengine_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_update_5fengine_2eproto();
#endif
  return *default_instance_;
}

DlcParameters* DlcParameters::default_instance_ = NULL;

DlcParameters* DlcParameters::New(::google::protobuf::Arena* arena) const {
  DlcParameters* n = new DlcParameters;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void DlcParameters::Clear() {
// @@protoc_insertion_point(message_clear_start:chromeos_update_engine.DlcParameters)
  omaha_url_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  dlc_infos_.Clear();
}

bool DlcParameters::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:chromeos_update_engine.DlcParameters)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .chromeos_update_engine.DlcInfo dlc_infos = 1;
      case 1: {
        if (tag == 10) {
          DO_(input->IncrementRecursionDepth());
         parse_loop_dlc_infos:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtualNoRecursionDepth(
                input, add_dlc_infos()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(10)) goto parse_loop_dlc_infos;
        input->UnsafeDecrementRecursionDepth();
        if (input->ExpectTag(18)) goto parse_omaha_url;
        break;
      }

      // optional string omaha_url = 2;
      case 2: {
        if (tag == 18) {
         parse_omaha_url:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_omaha_url()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->omaha_url().data(), this->omaha_url().length(),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "chromeos_update_engine.DlcParameters.omaha_url"));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:chromeos_update_engine.DlcParameters)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:chromeos_update_engine.DlcParameters)
  return false;
#undef DO_
}

void DlcParameters::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:chromeos_update_engine.DlcParameters)
  // repeated .chromeos_update_engine.DlcInfo dlc_infos = 1;
  for (unsigned int i = 0, n = this->dlc_infos_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      1, this->dlc_infos(i), output);
  }

  // optional string omaha_url = 2;
  if (this->omaha_url().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->omaha_url().data(), this->omaha_url().length(),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "chromeos_update_engine.DlcParameters.omaha_url");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->omaha_url(), output);
  }

  // @@protoc_insertion_point(serialize_end:chromeos_update_engine.DlcParameters)
}

int DlcParameters::ByteSize() const {
// @@protoc_insertion_point(message_byte_size_start:chromeos_update_engine.DlcParameters)
  int total_size = 0;

  // optional string omaha_url = 2;
  if (this->omaha_url().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->omaha_url());
  }

  // repeated .chromeos_update_engine.DlcInfo dlc_infos = 1;
  total_size += 1 * this->dlc_infos_size();
  for (int i = 0; i < this->dlc_infos_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->dlc_infos(i));
  }

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void DlcParameters::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const DlcParameters*>(&from));
}

void DlcParameters::MergeFrom(const DlcParameters& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:chromeos_update_engine.DlcParameters)
  if (GOOGLE_PREDICT_FALSE(&from == this)) {
    ::google::protobuf::internal::MergeFromFail(__FILE__, __LINE__);
  }
  dlc_infos_.MergeFrom(from.dlc_infos_);
  if (from.omaha_url().size() > 0) {

    omaha_url_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.omaha_url_);
  }
}

void DlcParameters::CopyFrom(const DlcParameters& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:chromeos_update_engine.DlcParameters)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DlcParameters::IsInitialized() const {

  return true;
}

void DlcParameters::Swap(DlcParameters* other) {
  if (other == this) return;
  InternalSwap(other);
}
void DlcParameters::InternalSwap(DlcParameters* other) {
  dlc_infos_.UnsafeArenaSwap(&other->dlc_infos_);
  omaha_url_.Swap(&other->omaha_url_);
  _unknown_fields_.Swap(&other->_unknown_fields_);
  std::swap(_cached_size_, other->_cached_size_);
}

::std::string DlcParameters::GetTypeName() const {
  return "chromeos_update_engine.DlcParameters";
}

#if PROTOBUF_INLINE_NOT_IN_HEADERS
// DlcParameters

// repeated .chromeos_update_engine.DlcInfo dlc_infos = 1;
int DlcParameters::dlc_infos_size() const {
  return dlc_infos_.size();
}
void DlcParameters::clear_dlc_infos() {
  dlc_infos_.Clear();
}
const ::chromeos_update_engine::DlcInfo& DlcParameters::dlc_infos(int index) const {
  // @@protoc_insertion_point(field_get:chromeos_update_engine.DlcParameters.dlc_infos)
  return dlc_infos_.Get(index);
}
::chromeos_update_engine::DlcInfo* DlcParameters::mutable_dlc_infos(int index) {
  // @@protoc_insertion_point(field_mutable:chromeos_update_engine.DlcParameters.dlc_infos)
  return dlc_infos_.Mutable(index);
}
::chromeos_update_engine::DlcInfo* DlcParameters::add_dlc_infos() {
  // @@protoc_insertion_point(field_add:chromeos_update_engine.DlcParameters.dlc_infos)
  return dlc_infos_.Add();
}
::google::protobuf::RepeatedPtrField< ::chromeos_update_engine::DlcInfo >*
DlcParameters::mutable_dlc_infos() {
  // @@protoc_insertion_point(field_mutable_list:chromeos_update_engine.DlcParameters.dlc_infos)
  return &dlc_infos_;
}
const ::google::protobuf::RepeatedPtrField< ::chromeos_update_engine::DlcInfo >&
DlcParameters::dlc_infos() const {
  // @@protoc_insertion_point(field_list:chromeos_update_engine.DlcParameters.dlc_infos)
  return dlc_infos_;
}

// optional string omaha_url = 2;
void DlcParameters::clear_omaha_url() {
  omaha_url_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 const ::std::string& DlcParameters::omaha_url() const {
  // @@protoc_insertion_point(field_get:chromeos_update_engine.DlcParameters.omaha_url)
  return omaha_url_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void DlcParameters::set_omaha_url(const ::std::string& value) {
  
  omaha_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:chromeos_update_engine.DlcParameters.omaha_url)
}
 void DlcParameters::set_omaha_url(const char* value) {
  
  omaha_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:chromeos_update_engine.DlcParameters.omaha_url)
}
 void DlcParameters::set_omaha_url(const char* value, size_t size) {
  
  omaha_url_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:chromeos_update_engine.DlcParameters.omaha_url)
}
 ::std::string* DlcParameters::mutable_omaha_url() {
  
  // @@protoc_insertion_point(field_mutable:chromeos_update_engine.DlcParameters.omaha_url)
  return omaha_url_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 ::std::string* DlcParameters::release_omaha_url() {
  // @@protoc_insertion_point(field_release:chromeos_update_engine.DlcParameters.omaha_url)
  
  return omaha_url_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
 void DlcParameters::set_allocated_omaha_url(::std::string* omaha_url) {
  if (omaha_url != NULL) {
    
  } else {
    
  }
  omaha_url_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), omaha_url);
  // @@protoc_insertion_point(field_set_allocated:chromeos_update_engine.DlcParameters.omaha_url)
}

#endif  // PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace chromeos_update_engine

// @@protoc_insertion_point(global_scope)
