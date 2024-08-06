// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MESSAGE_MYGAME_EXAMPLE_H_
#define FLATBUFFERS_GENERATED_MESSAGE_MYGAME_EXAMPLE_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 2 &&
              FLATBUFFERS_VERSION_MINOR == 0 &&
              FLATBUFFERS_VERSION_REVISION == 8,
             "Non-compatible flatbuffers version included");

namespace MyGame {
namespace Example {

struct Message;
struct MessageBuilder;

struct Message FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_TEXT = 6
  };
  int32_t id() const {
    return GetField<int32_t>(VT_ID, 0);
  }
  const flatbuffers::String *text() const {
    return GetPointer<const flatbuffers::String *>(VT_TEXT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID, 4) &&
           VerifyOffset(verifier, VT_TEXT) &&
           verifier.VerifyString(text()) &&
           verifier.EndTable();
  }
};

struct MessageBuilder {
  typedef Message Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(int32_t id) {
    fbb_.AddElement<int32_t>(Message::VT_ID, id, 0);
  }
  void add_text(flatbuffers::Offset<flatbuffers::String> text) {
    fbb_.AddOffset(Message::VT_TEXT, text);
  }
  explicit MessageBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<Message> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Message>(end);
    return o;
  }
};

inline flatbuffers::Offset<Message> CreateMessage(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    flatbuffers::Offset<flatbuffers::String> text = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_text(text);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<Message> CreateMessageDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t id = 0,
    const char *text = nullptr) {
  auto text__ = text ? _fbb.CreateString(text) : 0;
  return MyGame::Example::CreateMessage(
      _fbb,
      id,
      text__);
}

inline const MyGame::Example::Message *GetMessage(const void *buf) {
  return flatbuffers::GetRoot<MyGame::Example::Message>(buf);
}

inline const MyGame::Example::Message *GetSizePrefixedMessage(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<MyGame::Example::Message>(buf);
}

inline bool VerifyMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MyGame::Example::Message>(nullptr);
}

inline bool VerifySizePrefixedMessageBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MyGame::Example::Message>(nullptr);
}

inline void FinishMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<MyGame::Example::Message> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMessageBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<MyGame::Example::Message> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace Example
}  // namespace MyGame

#endif  // FLATBUFFERS_GENERATED_MESSAGE_MYGAME_EXAMPLE_H_
