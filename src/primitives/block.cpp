// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <primitives/block.h>

#include <hash.h>
#include <tinyformat.h>
#include <util/strencodings.h>
#include <crypto/common.h>
#include <crypto/blake3.h>

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CBlockHeader::GetPoWHash() const
{ 
  //Initialize a blake3_hasher in the key derivation mode
  blake3_hasher hasher;
  blake3_hasher_init_derive_key(&hasher, GetHash().ToString());
  
  // Finalize the hash. BLAKE3_OUT_LEN is the default output length, 32 bytes.
  uint8_t output[BLAKE3_OUT_LEN];
  blake3_hasher_finalize(&hasher, output, BLAKE3_OUT_LEN);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
