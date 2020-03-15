// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: trie.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_trie_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_trie_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3011000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3011004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_trie_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_trie_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_trie_2eproto;
namespace triepb {
class Node;
class NodeDefaultTypeInternal;
extern NodeDefaultTypeInternal _Node_default_instance_;
}  // namespace triepb
PROTOBUF_NAMESPACE_OPEN
template<> ::triepb::Node* Arena::CreateMaybeMessage<::triepb::Node>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace triepb {

// ===================================================================

class Node :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:triepb.Node) */ {
 public:
  Node();
  virtual ~Node();

  Node(const Node& from);
  Node(Node&& from) noexcept
    : Node() {
    *this = ::std::move(from);
  }

  inline Node& operator=(const Node& from) {
    CopyFrom(from);
    return *this;
  }
  inline Node& operator=(Node&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const Node& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Node* internal_default_instance() {
    return reinterpret_cast<const Node*>(
               &_Node_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Node& a, Node& b) {
    a.Swap(&b);
  }
  inline void Swap(Node* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Node* New() const final {
    return CreateMaybeMessage<Node>(nullptr);
  }

  Node* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Node>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Node& from);
  void MergeFrom(const Node& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Node* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "triepb.Node";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_trie_2eproto);
    return ::descriptor_table_trie_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kValFieldNumber = 1,
  };
  // repeated bytes val = 1;
  int val_size() const;
  private:
  int _internal_val_size() const;
  public:
  void clear_val();
  const std::string& val(int index) const;
  std::string* mutable_val(int index);
  void set_val(int index, const std::string& value);
  void set_val(int index, std::string&& value);
  void set_val(int index, const char* value);
  void set_val(int index, const void* value, size_t size);
  std::string* add_val();
  void add_val(const std::string& value);
  void add_val(std::string&& value);
  void add_val(const char* value);
  void add_val(const void* value, size_t size);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>& val() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>* mutable_val();
  private:
  const std::string& _internal_val(int index) const;
  std::string* _internal_add_val();
  public:

  // @@protoc_insertion_point(class_scope:triepb.Node)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string> val_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_trie_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Node

// repeated bytes val = 1;
inline int Node::_internal_val_size() const {
  return val_.size();
}
inline int Node::val_size() const {
  return _internal_val_size();
}
inline void Node::clear_val() {
  val_.Clear();
}
inline std::string* Node::add_val() {
  // @@protoc_insertion_point(field_add_mutable:triepb.Node.val)
  return _internal_add_val();
}
inline const std::string& Node::_internal_val(int index) const {
  return val_.Get(index);
}
inline const std::string& Node::val(int index) const {
  // @@protoc_insertion_point(field_get:triepb.Node.val)
  return _internal_val(index);
}
inline std::string* Node::mutable_val(int index) {
  // @@protoc_insertion_point(field_mutable:triepb.Node.val)
  return val_.Mutable(index);
}
inline void Node::set_val(int index, const std::string& value) {
  // @@protoc_insertion_point(field_set:triepb.Node.val)
  val_.Mutable(index)->assign(value);
}
inline void Node::set_val(int index, std::string&& value) {
  // @@protoc_insertion_point(field_set:triepb.Node.val)
  val_.Mutable(index)->assign(std::move(value));
}
inline void Node::set_val(int index, const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  val_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:triepb.Node.val)
}
inline void Node::set_val(int index, const void* value, size_t size) {
  val_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:triepb.Node.val)
}
inline std::string* Node::_internal_add_val() {
  return val_.Add();
}
inline void Node::add_val(const std::string& value) {
  val_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:triepb.Node.val)
}
inline void Node::add_val(std::string&& value) {
  val_.Add(std::move(value));
  // @@protoc_insertion_point(field_add:triepb.Node.val)
}
inline void Node::add_val(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  val_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:triepb.Node.val)
}
inline void Node::add_val(const void* value, size_t size) {
  val_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:triepb.Node.val)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>&
Node::val() const {
  // @@protoc_insertion_point(field_list:triepb.Node.val)
  return val_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField<std::string>*
Node::mutable_val() {
  // @@protoc_insertion_point(field_mutable_list:triepb.Node.val)
  return &val_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace triepb

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_trie_2eproto
