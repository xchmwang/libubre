// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: trie.proto

#include "trie.pb.h"

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
namespace triepb {
class NodeDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Node> _instance;
} _Node_default_instance_;
}  // namespace triepb
static void InitDefaultsscc_info_Node_trie_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::triepb::_Node_default_instance_;
    new (ptr) ::triepb::Node();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::triepb::Node::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Node_trie_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Node_trie_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_trie_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_trie_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_trie_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_trie_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::triepb::Node, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::triepb::Node, val_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::triepb::Node)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::triepb::_Node_default_instance_),
};

const char descriptor_table_protodef_trie_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ntrie.proto\022\006triepb\"\023\n\004Node\022\013\n\003val\030\001 \003("
  "\014b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_trie_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_trie_2eproto_sccs[1] = {
  &scc_info_Node_trie_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_trie_2eproto_once;
static bool descriptor_table_trie_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_trie_2eproto = {
  &descriptor_table_trie_2eproto_initialized, descriptor_table_protodef_trie_2eproto, "trie.proto", 49,
  &descriptor_table_trie_2eproto_once, descriptor_table_trie_2eproto_sccs, descriptor_table_trie_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_trie_2eproto::offsets,
  file_level_metadata_trie_2eproto, 1, file_level_enum_descriptors_trie_2eproto, file_level_service_descriptors_trie_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_trie_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_trie_2eproto)), true);
namespace triepb {

// ===================================================================

void Node::InitAsDefaultInstance() {
}
class Node::_Internal {
 public:
};

Node::Node()
  : ::PROTOBUF_NAMESPACE_ID::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:triepb.Node)
}
Node::Node(const Node& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _internal_metadata_(nullptr),
      val_(from.val_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:triepb.Node)
}

void Node::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Node_trie_2eproto.base);
}

Node::~Node() {
  // @@protoc_insertion_point(destructor:triepb.Node)
  SharedDtor();
}

void Node::SharedDtor() {
}

void Node::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Node& Node::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Node_trie_2eproto.base);
  return *internal_default_instance();
}


void Node::Clear() {
// @@protoc_insertion_point(message_clear_start:triepb.Node)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  val_.Clear();
  _internal_metadata_.Clear();
}

const char* Node::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated bytes val = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            auto str = _internal_add_val();
            ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* Node::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:triepb.Node)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated bytes val = 1;
  for (int i = 0, n = this->_internal_val_size(); i < n; i++) {
    const auto& s = this->_internal_val(i);
    target = stream->WriteBytes(1, s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:triepb.Node)
  return target;
}

size_t Node::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:triepb.Node)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated bytes val = 1;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(val_.size());
  for (int i = 0, n = val_.size(); i < n; i++) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
      val_.Get(i));
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Node::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:triepb.Node)
  GOOGLE_DCHECK_NE(&from, this);
  const Node* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Node>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:triepb.Node)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:triepb.Node)
    MergeFrom(*source);
  }
}

void Node::MergeFrom(const Node& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:triepb.Node)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  val_.MergeFrom(from.val_);
}

void Node::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:triepb.Node)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Node::CopyFrom(const Node& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:triepb.Node)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Node::IsInitialized() const {
  return true;
}

void Node::InternalSwap(Node* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  val_.InternalSwap(&other->val_);
}

::PROTOBUF_NAMESPACE_ID::Metadata Node::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace triepb
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::triepb::Node* Arena::CreateMaybeMessage< ::triepb::Node >(Arena* arena) {
  return Arena::CreateInternal< ::triepb::Node >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
