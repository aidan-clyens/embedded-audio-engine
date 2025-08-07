#ifndef __WAV_FILE_H__
#define __WAV_FILE_H__

#include <filesystem>
#include <memory>
#include <string>
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

  std::filesystem::path get_filepath() const
  {
    return m_filepath;
  }

  std::string get_filename() const
  {
    return m_filepath.filename().string();
  }

private:
  WavFile(const std::filesystem::path &path);

  std::filesystem::path m_filepath;

  SF_INFO m_sfinfo;
  std::shared_ptr<SNDFILE> m_sndfile;
};

}  // namespace Files

#endif  // __WAV_FILE_H__