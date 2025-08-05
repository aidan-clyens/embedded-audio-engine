#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <filesystem>
#include <vector>
#include <string>

namespace Files
{

/** @enum PathType
 *  @brief Enum to specify the type of path to filter when listing directory contents.
 */
enum class PathType
{
  Directory,
  File,
  All,
};

/** @class FileSystem
 *  @brief Singleton class for managing file system operations.
 */
class FileSystem
{
public:
  static FileSystem& instance()
  {
    static FileSystem instance;
    return instance;
  }
  
  void set_working_directory(const std::string &path);
  void set_working_directory(const std::filesystem::path &path);
  std::filesystem::path get_working_directory() const;

	std::vector<std::filesystem::path> list_directory(const std::filesystem::path &path, PathType type = PathType::All);

	std::vector<std::filesystem::path> list_wav_files_in_directory(const std::filesystem::path &path);

  /** @brief Checks if a specified path exists.
   *  @param path The path to check.
   *  @return True if the path exists, false otherwise.
   */
  inline bool path_exists(const std::filesystem::path &path) const
  {
    return std::filesystem::exists(path);
  }

  /** @brief Checks if a specified path is a file.
   *  @param path The path to check.
   *  @return True if the path is a file, false otherwise.
   */
  inline bool is_file(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_regular_file(path));
  }

  /** @brief Checks if a specified path is a WAV file.
   *  @param path The path to check.
   *  @return True if the path is a WAV file, false otherwise. 
   */
  inline bool is_wav_file(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_regular_file(path) && path.extension() == ".wav");
  }

  /** @brief Checks if a specified path is a directory.
   *  @param path The path to check.
   *  @return True if the path is a directory, false otherwise.
   */
  inline bool is_directory(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_directory(path));
  }

	void save_to_wav_file(std::vector<float> audio_buffer, const std::filesystem::path &path);
  std::vector<float> load_from_wav_file(const std::filesystem::path &path);

private:
  FileSystem() = default;
  virtual ~FileSystem() = default;

  // Disable copy and assignment
  FileSystem(const FileSystem&) = delete;
  FileSystem& operator=(const FileSystem&) = delete;

	std::filesystem::path m_working_directory;
};

}  // namespace Files

#endif  // __FILE_SYSTEM_H__