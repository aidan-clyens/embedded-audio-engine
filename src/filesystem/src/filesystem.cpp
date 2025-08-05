#include "filesystem.h"

#include <sndfile.h>

using namespace Files;

/** @brief Sets the working directory to the specified path.
 *  @param path The path to set as the working directory.
 *  @throws std::runtime_error if the path is invalid. 
 */
void FileSystem::set_working_directory(const std::string &path)
{
  if (!path_exists(std::filesystem::path(path)) || !is_directory(std::filesystem::path(path)))
  {
    throw std::runtime_error("Invalid working directory path: " + path);
  }

  m_working_directory = std::filesystem::path(path);
}

/** @brief Sets the working directory to the specified filesystem path.
 *  @param path The filesystem path to set as the working directory.
 *  @throws std::runtime_error if the path is invalid.
 */
void FileSystem::set_working_directory(const std::filesystem::path &path)
{
  if (!path_exists(path) || !is_directory(path))
  {
    throw std::runtime_error("Invalid working directory path: " + path.string());
  }

  m_working_directory = path;
}

/** @brief Gets the current working directory.
 *  @return The current working directory as a filesystem path.
 */
std::filesystem::path FileSystem::get_working_directory() const
{
  return m_working_directory;
}

/** @brief Lists the contents of a directory.
 *  @param path The path to the directory to list.
 *  @param type The type of contents to list (directories, files, or all).
 *  @return A vector of filesystem paths representing the contents of the directory.
 *  @throws std::runtime_error if the path does not exist or is not a directory.
 */ 
std::vector<std::filesystem::path> FileSystem::list_directory(const std::filesystem::path &path, PathType type)
{
  std::vector<std::filesystem::path> contents;

  if (!path_exists(path) || !is_directory(path))
  {
    throw std::runtime_error("Path does not exist or is not a directory: " + path.string());
  }

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    switch (type)
    {
      case PathType::Directory:
        if (is_directory(entry.path()))
          contents.push_back(entry.path());
        break;
      case PathType::File:
        if (is_file(entry.path()))
          contents.push_back(entry.path());
        break;
      case PathType::All:
        contents.push_back(entry.path());
        break;
      default:
        throw std::invalid_argument("Invalid PathType specified.");
    }
  }

  return contents;
}

/** @brief Lists WAV files in a specified directory.
 *  @param path The path to the directory to list.
 *  @return A vector of filesystem paths representing the WAV files in the specified directory.
 *  @throws std::runtime_error if the path does not exist or is not a directory
 */
std::vector<std::filesystem::path> FileSystem::list_wav_files_in_directory(const std::filesystem::path &path)
{
  std::vector<std::filesystem::path> contents;

  if (!path_exists(path) || !is_directory(path))
  {
    throw std::runtime_error("Path does not exist or is not a directory: " + path.string());
  }

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    if (is_wav_file(entry.path()))
    {
      contents.push_back(entry.path());
    }
  }

  return contents;
}

void FileSystem::save_to_wav_file(std::vector<float> audio_buffer, const std::filesystem::path &path)
{

}

/** @brief Loads audio data from a WAV file.
 *  @param path The path to the WAV file to load.
 *  @return A vector of floats representing the audio data.
 *  @throws std::runtime_error if the file cannot be opened or read.
 */
std::vector<float> FileSystem::load_from_wav_file(const std::filesystem::path &path)
{
  if (!path_exists(path) || !is_wav_file(path))
  {
    throw std::runtime_error("WAV file does not exist or is not a file: " + path.string());
  }

  SF_INFO sfinfo;
  SNDFILE *sndfile = sf_open("test.wav", SFM_READ, &sfinfo);
  std::vector<float> audioData(sfinfo.frames * sfinfo.channels);
  sf_readf_float(sndfile, audioData.data(), sfinfo.frames);
  sf_close(sndfile);
}
