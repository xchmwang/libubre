// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: dynasty.proto

#include "dynasty.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
extern PROTOBUF_INTERNAL_EXPORT_dynasty_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_DynastyCandidate_dynasty_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_dynasty_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_DynastyMeta_dynasty_2eproto;
namespace corepb {
class DynastyDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Dynasty> _instance;
} _Dynasty_default_instance_;
class DynastyMetaDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<DynastyMeta> _instance;
} _DynastyMeta_default_instance_;
class DynastyCandidateDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<DynastyCandidate> _instance;
} _DynastyCandidate_default_instance_;
}  // namespace corepb
static void InitDefaultsscc_info_Dynasty_dynasty_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::corepb::_Dynasty_default_instance_;
    new (ptr) ::corepb::Dynasty();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::corepb::Dynasty::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_Dynasty_dynasty_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 2, 0, InitDefaultsscc_info_Dynasty_dynasty_2eproto}, {
      &scc_info_DynastyMeta_dynasty_2eproto.base,
      &scc_info_DynastyCandidate_dynasty_2eproto.base,}};

static void InitDefaultsscc_info_DynastyCandidate_dynasty_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::corepb::_DynastyCandidate_default_instance_;
    new (ptr) ::corepb::DynastyCandidate();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::corepb::DynastyCandidate::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_DynastyCandidate_dynasty_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_DynastyCandidate_dynasty_2eproto}, {}};

static void InitDefaultsscc_info_DynastyMeta_dynasty_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::corepb::_DynastyMeta_default_instance_;
    new (ptr) ::corepb::DynastyMeta();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::corepb::DynastyMeta::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_DynastyMeta_dynasty_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_DynastyMeta_dynasty_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_dynasty_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_dynasty_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_dynasty_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_dynasty_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::corepb::Dynasty, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::corepb::Dynasty, meta_),
  PROTOBUF_FIELD_OFFSET(::corepb::Dynasty, candidate_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::corepb::DynastyMeta, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::corepb::DynastyMeta, chain_id_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::corepb::DynastyCandidate, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::corepb::DynastyCandidate, serial_),
  PROTOBUF_FIELD_OFFSET(::corepb::DynastyCandidate, dynasty_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::corepb::Dynasty)},
  { 7, -1, sizeof(::corepb::DynastyMeta)},
  { 13, -1, sizeof(::corepb::DynastyCandidate)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::corepb::_Dynasty_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::corepb::_DynastyMeta_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::corepb::_DynastyCandidate_default_instance_),
};

const char descriptor_table_protodef_dynasty_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rdynasty.proto\022\006corepb\"Y\n\007Dynasty\022!\n\004me"
  "ta\030\001 \001(\0132\023.corepb.DynastyMeta\022+\n\tcandida"
  "te\030\002 \003(\0132\030.corepb.DynastyCandidate\"\037\n\013Dy"
  "nastyMeta\022\020\n\010chain_id\030\001 \001(\r\"3\n\020DynastyCa"
  "ndidate\022\016\n\006serial\030\001 \001(\004\022\017\n\007dynasty\030\002 \003(\t"
  "b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_dynasty_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_dynasty_2eproto_sccs[3] = {
  &scc_info_Dynasty_dynasty_2eproto.base,
  &scc_info_DynastyCandidate_dynasty_2eproto.base,
  &scc_info_DynastyMeta_dynasty_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_dynasty_2eproto_once;
static bool descriptor_table_dynasty_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_dynasty_2eproto = {
  &descriptor_table_dynasty_2eproto_initialized, descriptor_table_protodef_dynasty_2eproto, "dynasty.proto", 208,
  &descriptor_table_dynasty_2eproto_once, descriptor_table_dynasty_2eproto_sccs, descriptor_table_dynasty_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_dynasty_2eproto::offsets,
  file_level_metadata_dynasty_2eproto, 3, file_level_enum_descriptors_dynasty_2eproto, file_level_service_descriptors_dynasty_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_dynasty_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_dynasty_2eproto)), true);
namespace corepb {

// ===================================================================

void Dynasty::InitAsDefaultInstance() {
  ::corepb::_Dynasty_default_instance_._instance.get_mutable()->meta_ = const_cast< ::corepb::DynastyMeta*>(
      ::corepb::DynastyMeta::internal_default_instance());
}
class Dynasty::_Internal {
 public:
  static const ::corepb::DynastyMeta& meta(const Dynasty* msg);
};

const ::corepb::DynastyMeta&
Dynasty::_Internal::meta(const Dynasty* msg) {
  return *msg->meta_;
}
Dynasty::Dynasty()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:corepb.Dynasty)
}
Dynasty::Dynasty(const Dynasty& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      candidate_(from.candidate_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  if (from._internal_has_meta()) {
    meta_ = new ::corepb::DynastyMeta(*from.meta_);
  } else {
    meta_ = nullptr;
  }
  // @@protoc_insertion_point(copy_constructor:corepb.Dynasty)
}

void Dynasty::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Dynasty_dynasty_2eproto.base);
  meta_ = nullptr;
}

Dynasty::~Dynasty() {
  // @@protoc_insertion_point(destructor:corepb.Dynasty)
  SharedDtor();
}

void Dynasty::SharedDtor() {
  if (this != internal_default_instance()) delete meta_;
}

void Dynasty::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Dynasty& Dynasty::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Dynasty_dynasty_2eproto.base);
  return *internal_default_instance();
}


void Dynasty::Clear() {
// @@protoc_insertion_point(message_clear_start:corepb.Dynasty)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  candidate_.Clear();
  if (GetArenaNoVirtual() == nullptr && meta_ != nullptr) {
    delete meta_;
  }
  meta_ = nullptr;
  _internal_metadata_.Clear();
}

const char* Dynasty::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // .corepb.DynastyMeta meta = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ctx->ParseMessage(_internal_mutable_meta(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated .corepb.DynastyCandidate candidate = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_candidate(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Dynasty::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:corepb.Dynasty)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // .corepb.DynastyMeta meta = 1;
  if (this->has_meta()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        1, _Internal::meta(this), target, stream);
  }

  // repeated .corepb.DynastyCandidate candidate = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_candidate_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, this->_internal_candidate(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:corepb.Dynasty)
  return target;
}

size_t Dynasty::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:corepb.Dynasty)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .corepb.DynastyCandidate candidate = 2;
  total_size += 1UL * this->_internal_candidate_size();
  for (const auto& msg : this->candidate_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // .corepb.DynastyMeta meta = 1;
  if (this->has_meta()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *meta_);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Dynasty::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:corepb.Dynasty)
  GOOGLE_DCHECK_NE(&from, this);
  const Dynasty* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Dynasty>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:corepb.Dynasty)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:corepb.Dynasty)
    MergeFrom(*source);
  }
}

void Dynasty::MergeFrom(const Dynasty& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:corepb.Dynasty)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  candidate_.MergeFrom(from.candidate_);
  if (from.has_meta()) {
    _internal_mutable_meta()->::corepb::DynastyMeta::MergeFrom(from._internal_meta());
  }
}

void Dynasty::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:corepb.Dynasty)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Dynasty::CopyFrom(const Dynasty& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:corepb.Dynasty)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Dynasty::IsInitialized() const {
  return true;
}

void Dynasty::InternalSwap(Dynasty* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  candidate_.InternalSwap(&other->candidate_);
  swap(meta_, other->meta_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Dynasty::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void DynastyMeta::InitAsDefaultInstance() {
}
class DynastyMeta::_Internal {
 public:
};

DynastyMeta::DynastyMeta()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:corepb.DynastyMeta)
}
DynastyMeta::DynastyMeta(const DynastyMeta& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  chain_id_ = from.chain_id_;
  // @@protoc_insertion_point(copy_constructor:corepb.DynastyMeta)
}

void DynastyMeta::SharedCtor() {
  chain_id_ = 0u;
}

DynastyMeta::~DynastyMeta() {
  // @@protoc_insertion_point(destructor:corepb.DynastyMeta)
  SharedDtor();
}

void DynastyMeta::SharedDtor() {
}

void DynastyMeta::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const DynastyMeta& DynastyMeta::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_DynastyMeta_dynasty_2eproto.base);
  return *internal_default_instance();
}


void DynastyMeta::Clear() {
// @@protoc_insertion_point(message_clear_start:corepb.DynastyMeta)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  chain_id_ = 0u;
  _internal_metadata_.Clear();
}

const char* DynastyMeta::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint32 chain_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          chain_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* DynastyMeta::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:corepb.DynastyMeta)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint32 chain_id = 1;
  if (this->chain_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_chain_id(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:corepb.DynastyMeta)
  return target;
}

size_t DynastyMeta::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:corepb.DynastyMeta)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint32 chain_id = 1;
  if (this->chain_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_chain_id());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void DynastyMeta::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:corepb.DynastyMeta)
  GOOGLE_DCHECK_NE(&from, this);
  const DynastyMeta* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<DynastyMeta>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:corepb.DynastyMeta)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:corepb.DynastyMeta)
    MergeFrom(*source);
  }
}

void DynastyMeta::MergeFrom(const DynastyMeta& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:corepb.DynastyMeta)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.chain_id() != 0) {
    _internal_set_chain_id(from._internal_chain_id());
  }
}

void DynastyMeta::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:corepb.DynastyMeta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void DynastyMeta::CopyFrom(const DynastyMeta& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:corepb.DynastyMeta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DynastyMeta::IsInitialized() const {
  return true;
}

void DynastyMeta::InternalSwap(DynastyMeta* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(chain_id_, other->chain_id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DynastyMeta::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void DynastyCandidate::InitAsDefaultInstance() {
}
class DynastyCandidate::_Internal {
 public:
};

DynastyCandidate::DynastyCandidate()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:corepb.DynastyCandidate)
}
DynastyCandidate::DynastyCandidate(const DynastyCandidate& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      dynasty_(from.dynasty_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  serial_ = from.serial_;
  // @@protoc_insertion_point(copy_constructor:corepb.DynastyCandidate)
}

void DynastyCandidate::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_DynastyCandidate_dynasty_2eproto.base);
  serial_ = PROTOBUF_ULONGLONG(0);
}

DynastyCandidate::~DynastyCandidate() {
  // @@protoc_insertion_point(destructor:corepb.DynastyCandidate)
  SharedDtor();
}

void DynastyCandidate::SharedDtor() {
}

void DynastyCandidate::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const DynastyCandidate& DynastyCandidate::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_DynastyCandidate_dynasty_2eproto.base);
  return *internal_default_instance();
}


void DynastyCandidate::Clear() {
// @@protoc_insertion_point(message_clear_start:corepb.DynastyCandidate)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  dynasty_.Clear();
  serial_ = PROTOBUF_ULONGLONG(0);
  _internal_metadata_.Clear();
}

const char* DynastyCandidate::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint64 serial = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          serial_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // repeated string dynasty = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_dynasty();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "corepb.DynastyCandidate.dynasty"));
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag, &_internal_metadata_, ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* DynastyCandidate::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:corepb.DynastyCandidate)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 serial = 1;
  if (this->serial() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_serial(), target);
  }

  // repeated string dynasty = 2;
  for (int i = 0, n = this->_internal_dynasty_size(); i < n; i++) {
    const auto& s = this->_internal_dynasty(i);
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      s.data(), static_cast<int>(s.length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "corepb.DynastyCandidate.dynasty");
    target = stream->WriteString(2, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:corepb.DynastyCandidate)
  return target;
}

size_t DynastyCandidate::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:corepb.DynastyCandidate)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated string dynasty = 2;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(dynasty_.size());
  for (int i = 0, n = dynasty_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
      dynasty_.Get(i));
  }

  // uint64 serial = 1;
  if (this->serial() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_serial());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void DynastyCandidate::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:corepb.DynastyCandidate)
  GOOGLE_DCHECK_NE(&from, this);
  const DynastyCandidate* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<DynastyCandidate>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:corepb.DynastyCandidate)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:corepb.DynastyCandidate)
    MergeFrom(*source);
  }
}

void DynastyCandidate::MergeFrom(const DynastyCandidate& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:corepb.DynastyCandidate)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  dynasty_.MergeFrom(from.dynasty_);
  if (from.serial() != 0) {
    _internal_set_serial(from._internal_serial());
  }
}

void DynastyCandidate::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:corepb.DynastyCandidate)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void DynastyCandidate::CopyFrom(const DynastyCandidate& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:corepb.DynastyCandidate)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DynastyCandidate::IsInitialized() const {
  return true;
}

void DynastyCandidate::InternalSwap(DynastyCandidate* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  dynasty_.InternalSwap(&other->dynasty_);
  swap(serial_, other->serial_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DynastyCandidate::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace corepb
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::corepb::Dynasty* Arena::CreateMaybeMessage< ::corepb::Dynasty >(Arena* arena) {
  return Arena::CreateInternal< ::corepb::Dynasty >(arena);
}
template<> PROTOBUF_NOINLINE ::corepb::DynastyMeta* Arena::CreateMaybeMessage< ::corepb::DynastyMeta >(Arena* arena) {
  return Arena::CreateInternal< ::corepb::DynastyMeta >(arena);
}
template<> PROTOBUF_NOINLINE ::corepb::DynastyCandidate* Arena::CreateMaybeMessage< ::corepb::DynastyCandidate >(Arena* arena) {
  return Arena::CreateInternal< ::corepb::DynastyCandidate >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
