// Copyright 2011 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Declares the AddPdbInfoTransform. This adorns a PE BlockGraph with a
// debug data directory entry, which contains information regarding the PDB file
// for this block-graph (actually, for the module that will be created from it).
//
// If the module already contains a debug directory entry it will be modified
// in-place.

#ifndef SYZYGY_PE_TRANSFORMS_ADD_PDB_INFO_TRANSFORM_H_
#define SYZYGY_PE_TRANSFORMS_ADD_PDB_INFO_TRANSFORM_H_

#include "base/file_path.h"
#include "syzygy/block_graph/transforms/named_transform.h"

namespace pe {
namespace transforms {

using block_graph::BlockGraph;
using block_graph::transforms::NamedTransformImpl;

// A PE BlockGraph transform for adding/updating the a debug directory entry
// of a given type.
class AddPdbInfoTransform
    : public NamedTransformImpl<AddPdbInfoTransform> {
 public:
  // Configures this transform.
  //
  // @param pdb_path the path of the PDB file to link to.
  // @param pdb_age the age of the PDB file to link to.
  // @param pdb_guid the GUID of the PDB file to link to.
  AddPdbInfoTransform(const FilePath& pdb_path,
                      uint32 pdb_age,
                      const GUID& pdb_guid)
      : pdb_path_(pdb_path), pdb_age_(pdb_age), pdb_guid_(pdb_guid) {
  }

  // Adds or finds the debug data directory of the given type.
  //
  // @param block_graph The block graph to transform.
  // @param dos_header_block The DOS header block of the block graph.
  // @returns true on success, false otherwise.
  virtual bool Apply(BlockGraph* block_graph,
                     BlockGraph::Block* dos_header_block);

  // The tranform name.
  static const char kTransformName[];

 private:
  FilePath pdb_path_;
  uint32 pdb_age_;
  GUID pdb_guid_;

  DISALLOW_COPY_AND_ASSIGN(AddPdbInfoTransform);
};

}  // namespace transforms
}  // namespace pe

#endif  // SYZYGY_PE_TRANSFORMS_ADD_PDB_INFO_TRANSFORM_H_
