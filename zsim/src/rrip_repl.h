#ifndef RRIP_REPL_H_
#define RRIP_REPL_H_

#include "repl_policies.h"

// Static RRIP
class SRRIPReplPolicy : public ReplPolicy {
protected:
  uint32_t *array;
  uint32_t numLines;
  uint32_t rpvMax;

public:
  explicit SRRIPReplPolicy(uint32_t _numLines, uint32_t _rpvMax)
      : numLines(_numLines), rpvMax(_rpvMax) {
    array = gm_calloc<uint32_t>(numLines);
  }

  ~SRRIPReplPolicy() { gm_free(array); }

  void inline update(uint32_t id, const MemReq *req) { array[id] = 0; }

  void replaced(uint32_t id) {
    // No-op because nothing is required in this case
  }

  template <typename C> inline uint32_t rank(const MemReq *req, C cands) {
    for (uint32_t i = 0; i < rpvMax; i++) {
      for (auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
        if (array[*ci] == rpvMax) {
          return *ci;
        }
      }

      for (auto ci = cands.begin(); ci != cands.end(); ci.inc()) {
        array[*ci]++;
      }
    }
  }

  DECL_RANK_BINDINGS;
};
#endif // RRIP_REPL_H_
