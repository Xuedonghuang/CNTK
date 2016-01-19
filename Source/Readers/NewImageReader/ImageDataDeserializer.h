//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#pragma once

#include "BaseDataDeserializer.h"
#include "commandArgUtil.h"
#include <opencv2/core/mat.hpp>

namespace Microsoft { namespace MSR { namespace CNTK {

// Image data deserializer based on the OpenCV library.
// The deserializer currently supports two output streams only: a feature and a label stream.
// All sequences consist only of a single sample (image/label).
// For features it uses dense storage format with different layour (dimensions) between different sequences.
// For labels it uses sparse storage format.
class ImageDataDeserializer : public BaseDataDeserializer
{
public:
    explicit ImageDataDeserializer(const ConfigParameters& config);

    // Description of streams that this data deserializer provides.
    std::vector<StreamDescriptionPtr> GetStreams() const override;

    // Get sequences by specified ids. Order of returned sequences correponds to the order of provided ids.
    std::vector<std::vector<SequenceDataPtr>> GetSequencesById(const std::vector<size_t>& ids) override;

protected:
    void FillSequenceDescriptions(Timeline& timeline) const override;

private:
    // Creates a set of sequence descriptions.
    void CreateSequenceDescriptions(std::string mapPath, size_t labelDimension);

    // Image sequence descriptions. Currently, a sequence contains a single sample only.
    struct ImageSequenceDescription : public SequenceDescription
    {
        std::string path;
        size_t classId;
    };

    // A helper class for generation of type specific labels (currently float/double only).
    class LabelGenerator;
    typedef std::shared_ptr<LabelGenerator> LabelGeneratorPtr;
    LabelGeneratorPtr m_labelGenerator;

    // Sequence descriptions for all input data.
    std::vector<ImageSequenceDescription> m_imageSequences;

    // Buffer to store label data.
    std::vector<SparseSequenceDataPtr> m_labels;

    // Buffer to store feature data.
    std::vector<cv::Mat> m_currentImages;

    // Element type of the feature/label stream (currently float/double only).
    ElementType m_featureElementType;
};
} } }
