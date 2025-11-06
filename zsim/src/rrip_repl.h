#ifndef RRIP_REPL_H_
#define RRIP_REPL_H_

#include "repl_policies.h"

// Static RRIP
class SRRIPReplPolicy : public ReplPolicy {
protected:
  uint32_t *array;
  bool *isReplaced;
  uint32_t numLines;
  uint32_t rpvMax;

public:
  explicit SRRIPReplPolicy(uint32_t _numLines, uint32_t _rpvMax)
      : numLines(_numLines), rpvMax(_rpvMax) {
    array = gm_calloc<uint32_t>(numLines);
    isReplaced = gm_calloc<bool>(numLines);
    for (uint32_t i = 0; i < numLines; i++) {
      array[i] = rpvMax;
    }
  }

  ~SRRIPReplPolicy() {
    gm_free(array);
    gm_free(isReplaced);
  }

  void inline update(uint32_t id, const MemReq *req) {
    if (isReplaced[id]) {
      array[id] = rpvMax - 1;
      isReplaced[id] = false;
    } else {
      array[id] = 0;
    }
  }

  void replaced(uint32_t id) { isReplaced[id] = true; }

  template <typename C> inline uint32_t rank(const MemReq *req, C cands) {
    while (true) {
      for (auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
        uint32_t id = *ci;
        if (array[id] == rpvMax) {
          return id;
        }
      }
      for (auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
        array[*ci]++;
      }
    }

	return 0;
  }

  DECL_RANK_BINDINGS;
};

#endif //RRIP_REPL_H
