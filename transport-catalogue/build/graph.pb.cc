// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: graph.proto

#include "graph.pb.h"

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

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace graph_proto {
PROTOBUF_CONSTEXPR Edge::Edge(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.bus_name_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.from_)*/uint64_t{0u}
  , /*decltype(_impl_.to_)*/uint64_t{0u}
  , /*decltype(_impl_.weight_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct EdgeDefaultTypeInternal {
  PROTOBUF_CONSTEXPR EdgeDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~EdgeDefaultTypeInternal() {}
  union {
    Edge _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 EdgeDefaultTypeInternal _Edge_default_instance_;
PROTOBUF_CONSTEXPR IncidenceList::IncidenceList(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.edge_id_)*/{}
  , /*decltype(_impl_._edge_id_cached_byte_size_)*/{0}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct IncidenceListDefaultTypeInternal {
  PROTOBUF_CONSTEXPR IncidenceListDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~IncidenceListDefaultTypeInternal() {}
  union {
    IncidenceList _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 IncidenceListDefaultTypeInternal _IncidenceList_default_instance_;
PROTOBUF_CONSTEXPR DirectedWeightedGraph::DirectedWeightedGraph(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.edge_)*/{}
  , /*decltype(_impl_.incidence_list_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct DirectedWeightedGraphDefaultTypeInternal {
  PROTOBUF_CONSTEXPR DirectedWeightedGraphDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~DirectedWeightedGraphDefaultTypeInternal() {}
  union {
    DirectedWeightedGraph _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 DirectedWeightedGraphDefaultTypeInternal _DirectedWeightedGraph_default_instance_;
}  // namespace graph_proto
static ::_pb::Metadata file_level_metadata_graph_2eproto[3];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_graph_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_graph_2eproto = nullptr;

const uint32_t TableStruct_graph_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::graph_proto::Edge, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::graph_proto::Edge, _impl_.from_),
  PROTOBUF_FIELD_OFFSET(::graph_proto::Edge, _impl_.to_),
  PROTOBUF_FIELD_OFFSET(::graph_proto::Edge, _impl_.weight_),
  PROTOBUF_FIELD_OFFSET(::graph_proto::Edge, _impl_.bus_name_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::graph_proto::IncidenceList, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::graph_proto::IncidenceList, _impl_.edge_id_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::graph_proto::DirectedWeightedGraph, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::graph_proto::DirectedWeightedGraph, _impl_.edge_),
  PROTOBUF_FIELD_OFFSET(::graph_proto::DirectedWeightedGraph, _impl_.incidence_list_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::graph_proto::Edge)},
  { 10, -1, -1, sizeof(::graph_proto::IncidenceList)},
  { 17, -1, -1, sizeof(::graph_proto::DirectedWeightedGraph)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::graph_proto::_Edge_default_instance_._instance,
  &::graph_proto::_IncidenceList_default_instance_._instance,
  &::graph_proto::_DirectedWeightedGraph_default_instance_._instance,
};

const char descriptor_table_protodef_graph_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\013graph.proto\022\013graph_proto\"B\n\004Edge\022\014\n\004fr"
  "om\030\001 \001(\004\022\n\n\002to\030\002 \001(\004\022\016\n\006weight\030\003 \001(\001\022\020\n\010"
  "bus_name\030\004 \001(\014\" \n\rIncidenceList\022\017\n\007edge_"
  "id\030\001 \003(\005\"l\n\025DirectedWeightedGraph\022\037\n\004edg"
  "e\030\001 \003(\0132\021.graph_proto.Edge\0222\n\016incidence_"
  "list\030\002 \003(\0132\032.graph_proto.IncidenceListb\006"
  "proto3"
  ;
static ::_pbi::once_flag descriptor_table_graph_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_graph_2eproto = {
    false, false, 246, descriptor_table_protodef_graph_2eproto,
    "graph.proto",
    &descriptor_table_graph_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_graph_2eproto::offsets,
    file_level_metadata_graph_2eproto, file_level_enum_descriptors_graph_2eproto,
    file_level_service_descriptors_graph_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_graph_2eproto_getter() {
  return &descriptor_table_graph_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_graph_2eproto(&descriptor_table_graph_2eproto);
namespace graph_proto {

// ===================================================================

class Edge::_Internal {
 public:
};

Edge::Edge(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:graph_proto.Edge)
}
Edge::Edge(const Edge& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  Edge* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.bus_name_){}
    , decltype(_impl_.from_){}
    , decltype(_impl_.to_){}
    , decltype(_impl_.weight_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.bus_name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.bus_name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_bus_name().empty()) {
    _this->_impl_.bus_name_.Set(from._internal_bus_name(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.from_, &from._impl_.from_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.weight_) -
    reinterpret_cast<char*>(&_impl_.from_)) + sizeof(_impl_.weight_));
  // @@protoc_insertion_point(copy_constructor:graph_proto.Edge)
}

inline void Edge::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.bus_name_){}
    , decltype(_impl_.from_){uint64_t{0u}}
    , decltype(_impl_.to_){uint64_t{0u}}
    , decltype(_impl_.weight_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.bus_name_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.bus_name_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

Edge::~Edge() {
  // @@protoc_insertion_point(destructor:graph_proto.Edge)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void Edge::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.bus_name_.Destroy();
}

void Edge::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void Edge::Clear() {
// @@protoc_insertion_point(message_clear_start:graph_proto.Edge)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.bus_name_.ClearToEmpty();
  ::memset(&_impl_.from_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.weight_) -
      reinterpret_cast<char*>(&_impl_.from_)) + sizeof(_impl_.weight_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Edge::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 from = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.from_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // uint64 to = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.to_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // double weight = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 25)) {
          _impl_.weight_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<double>(ptr);
          ptr += sizeof(double);
        } else
          goto handle_unusual;
        continue;
      // bytes bus_name = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 34)) {
          auto str = _internal_mutable_bus_name();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* Edge::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:graph_proto.Edge)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 from = 1;
  if (this->_internal_from() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(1, this->_internal_from(), target);
  }

  // uint64 to = 2;
  if (this->_internal_to() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(2, this->_internal_to(), target);
  }

  // double weight = 3;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_weight = this->_internal_weight();
  uint64_t raw_weight;
  memcpy(&raw_weight, &tmp_weight, sizeof(tmp_weight));
  if (raw_weight != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteDoubleToArray(3, this->_internal_weight(), target);
  }

  // bytes bus_name = 4;
  if (!this->_internal_bus_name().empty()) {
    target = stream->WriteBytesMaybeAliased(
        4, this->_internal_bus_name(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:graph_proto.Edge)
  return target;
}

size_t Edge::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:graph_proto.Edge)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // bytes bus_name = 4;
  if (!this->_internal_bus_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
        this->_internal_bus_name());
  }

  // uint64 from = 1;
  if (this->_internal_from() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_from());
  }

  // uint64 to = 2;
  if (this->_internal_to() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_to());
  }

  // double weight = 3;
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_weight = this->_internal_weight();
  uint64_t raw_weight;
  memcpy(&raw_weight, &tmp_weight, sizeof(tmp_weight));
  if (raw_weight != 0) {
    total_size += 1 + 8;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Edge::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    Edge::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Edge::GetClassData() const { return &_class_data_; }


void Edge::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<Edge*>(&to_msg);
  auto& from = static_cast<const Edge&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:graph_proto.Edge)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_bus_name().empty()) {
    _this->_internal_set_bus_name(from._internal_bus_name());
  }
  if (from._internal_from() != 0) {
    _this->_internal_set_from(from._internal_from());
  }
  if (from._internal_to() != 0) {
    _this->_internal_set_to(from._internal_to());
  }
  static_assert(sizeof(uint64_t) == sizeof(double), "Code assumes uint64_t and double are the same size.");
  double tmp_weight = from._internal_weight();
  uint64_t raw_weight;
  memcpy(&raw_weight, &tmp_weight, sizeof(tmp_weight));
  if (raw_weight != 0) {
    _this->_internal_set_weight(from._internal_weight());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void Edge::CopyFrom(const Edge& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:graph_proto.Edge)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Edge::IsInitialized() const {
  return true;
}

void Edge::InternalSwap(Edge* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.bus_name_, lhs_arena,
      &other->_impl_.bus_name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Edge, _impl_.weight_)
      + sizeof(Edge::_impl_.weight_)
      - PROTOBUF_FIELD_OFFSET(Edge, _impl_.from_)>(
          reinterpret_cast<char*>(&_impl_.from_),
          reinterpret_cast<char*>(&other->_impl_.from_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Edge::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_graph_2eproto_getter, &descriptor_table_graph_2eproto_once,
      file_level_metadata_graph_2eproto[0]);
}

// ===================================================================

class IncidenceList::_Internal {
 public:
};

IncidenceList::IncidenceList(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:graph_proto.IncidenceList)
}
IncidenceList::IncidenceList(const IncidenceList& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  IncidenceList* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.edge_id_){from._impl_.edge_id_}
    , /*decltype(_impl_._edge_id_cached_byte_size_)*/{0}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:graph_proto.IncidenceList)
}

inline void IncidenceList::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.edge_id_){arena}
    , /*decltype(_impl_._edge_id_cached_byte_size_)*/{0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

IncidenceList::~IncidenceList() {
  // @@protoc_insertion_point(destructor:graph_proto.IncidenceList)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void IncidenceList::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.edge_id_.~RepeatedField();
}

void IncidenceList::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void IncidenceList::Clear() {
// @@protoc_insertion_point(message_clear_start:graph_proto.IncidenceList)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.edge_id_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* IncidenceList::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated int32 edge_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedInt32Parser(_internal_mutable_edge_id(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<uint8_t>(tag) == 8) {
          _internal_add_edge_id(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* IncidenceList::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:graph_proto.IncidenceList)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated int32 edge_id = 1;
  {
    int byte_size = _impl_._edge_id_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteInt32Packed(
          1, _internal_edge_id(), byte_size, target);
    }
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:graph_proto.IncidenceList)
  return target;
}

size_t IncidenceList::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:graph_proto.IncidenceList)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated int32 edge_id = 1;
  {
    size_t data_size = ::_pbi::WireFormatLite::
      Int32Size(this->_impl_.edge_id_);
    if (data_size > 0) {
      total_size += 1 +
        ::_pbi::WireFormatLite::Int32Size(static_cast<int32_t>(data_size));
    }
    int cached_size = ::_pbi::ToCachedSize(data_size);
    _impl_._edge_id_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData IncidenceList::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    IncidenceList::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*IncidenceList::GetClassData() const { return &_class_data_; }


void IncidenceList::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<IncidenceList*>(&to_msg);
  auto& from = static_cast<const IncidenceList&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:graph_proto.IncidenceList)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.edge_id_.MergeFrom(from._impl_.edge_id_);
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void IncidenceList::CopyFrom(const IncidenceList& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:graph_proto.IncidenceList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool IncidenceList::IsInitialized() const {
  return true;
}

void IncidenceList::InternalSwap(IncidenceList* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.edge_id_.InternalSwap(&other->_impl_.edge_id_);
}

::PROTOBUF_NAMESPACE_ID::Metadata IncidenceList::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_graph_2eproto_getter, &descriptor_table_graph_2eproto_once,
      file_level_metadata_graph_2eproto[1]);
}

// ===================================================================

class DirectedWeightedGraph::_Internal {
 public:
};

DirectedWeightedGraph::DirectedWeightedGraph(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:graph_proto.DirectedWeightedGraph)
}
DirectedWeightedGraph::DirectedWeightedGraph(const DirectedWeightedGraph& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  DirectedWeightedGraph* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.edge_){from._impl_.edge_}
    , decltype(_impl_.incidence_list_){from._impl_.incidence_list_}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:graph_proto.DirectedWeightedGraph)
}

inline void DirectedWeightedGraph::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.edge_){arena}
    , decltype(_impl_.incidence_list_){arena}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

DirectedWeightedGraph::~DirectedWeightedGraph() {
  // @@protoc_insertion_point(destructor:graph_proto.DirectedWeightedGraph)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void DirectedWeightedGraph::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.edge_.~RepeatedPtrField();
  _impl_.incidence_list_.~RepeatedPtrField();
}

void DirectedWeightedGraph::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void DirectedWeightedGraph::Clear() {
// @@protoc_insertion_point(message_clear_start:graph_proto.DirectedWeightedGraph)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.edge_.Clear();
  _impl_.incidence_list_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* DirectedWeightedGraph::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // repeated .graph_proto.Edge edge = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_edge(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
        } else
          goto handle_unusual;
        continue;
      // repeated .graph_proto.IncidenceList incidence_list = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_incidence_list(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* DirectedWeightedGraph::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:graph_proto.DirectedWeightedGraph)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .graph_proto.Edge edge = 1;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_edge_size()); i < n; i++) {
    const auto& repfield = this->_internal_edge(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(1, repfield, repfield.GetCachedSize(), target, stream);
  }

  // repeated .graph_proto.IncidenceList incidence_list = 2;
  for (unsigned i = 0,
      n = static_cast<unsigned>(this->_internal_incidence_list_size()); i < n; i++) {
    const auto& repfield = this->_internal_incidence_list(i);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(2, repfield, repfield.GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:graph_proto.DirectedWeightedGraph)
  return target;
}

size_t DirectedWeightedGraph::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:graph_proto.DirectedWeightedGraph)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .graph_proto.Edge edge = 1;
  total_size += 1UL * this->_internal_edge_size();
  for (const auto& msg : this->_impl_.edge_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  // repeated .graph_proto.IncidenceList incidence_list = 2;
  total_size += 1UL * this->_internal_incidence_list_size();
  for (const auto& msg : this->_impl_.incidence_list_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData DirectedWeightedGraph::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    DirectedWeightedGraph::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*DirectedWeightedGraph::GetClassData() const { return &_class_data_; }


void DirectedWeightedGraph::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<DirectedWeightedGraph*>(&to_msg);
  auto& from = static_cast<const DirectedWeightedGraph&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:graph_proto.DirectedWeightedGraph)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  _this->_impl_.edge_.MergeFrom(from._impl_.edge_);
  _this->_impl_.incidence_list_.MergeFrom(from._impl_.incidence_list_);
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void DirectedWeightedGraph::CopyFrom(const DirectedWeightedGraph& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:graph_proto.DirectedWeightedGraph)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool DirectedWeightedGraph::IsInitialized() const {
  return true;
}

void DirectedWeightedGraph::InternalSwap(DirectedWeightedGraph* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  _impl_.edge_.InternalSwap(&other->_impl_.edge_);
  _impl_.incidence_list_.InternalSwap(&other->_impl_.incidence_list_);
}

::PROTOBUF_NAMESPACE_ID::Metadata DirectedWeightedGraph::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_graph_2eproto_getter, &descriptor_table_graph_2eproto_once,
      file_level_metadata_graph_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace graph_proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::graph_proto::Edge*
Arena::CreateMaybeMessage< ::graph_proto::Edge >(Arena* arena) {
  return Arena::CreateMessageInternal< ::graph_proto::Edge >(arena);
}
template<> PROTOBUF_NOINLINE ::graph_proto::IncidenceList*
Arena::CreateMaybeMessage< ::graph_proto::IncidenceList >(Arena* arena) {
  return Arena::CreateMessageInternal< ::graph_proto::IncidenceList >(arena);
}
template<> PROTOBUF_NOINLINE ::graph_proto::DirectedWeightedGraph*
Arena::CreateMaybeMessage< ::graph_proto::DirectedWeightedGraph >(Arena* arena) {
  return Arena::CreateMessageInternal< ::graph_proto::DirectedWeightedGraph >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>