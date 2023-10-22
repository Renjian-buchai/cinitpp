#if !defined(ENUM_HH_)
#define ENUM_HH_

enum exitVal {
  success,
  nonEmptyDir,
  configFileCreationFailed,
  configJsonInvalid,
  fileCreationFailed,
  dirCreationFailed,
  deletionFailed,
  testerr
};

#endif  // ENUM_HH_
