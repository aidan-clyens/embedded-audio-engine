#ifndef __WAV_FILE_H__
#define __WAV_FILE_H__

#include <filesystem>
#include <memory>
#include <sndfile.h>

namespace Files
{

/** @class AudioFile
 *  @brief Class for handling audio file operations.
 */
class WavFile
{
friend class FileSystem;

public:
  virtual ~WavFile() = default;

private:
  WavFile(const std::filesystem::path &path);

  SF_INFO m_sfinfo;
  std::shared_ptr<SNDFILE> m_sndfile;
};

}  // namespace Files

#endif  // __WAV_FILE_H__