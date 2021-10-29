pub mod parse{
    use std::fs;

    pub enum ArgType{
        DirPath(String),
    }

    pub read_env(args: Vec<String>){
        let args: Vec<ArgType> = vec![];
        
    }

    pub fn parse_dir(path_dir: String) -> Vec<String> {
        let mut path_files: Vec<String> = vec![];

        for path in fs::read_dir(path_dir).unwrap() {
            let new_path = path.unwrap();
            if new_path.metadata().unwrap().is_dir() {
                path_files.append(&mut parse_dir(new_path.path().display().to_string()));
            } else {
                if new_path.path().display().to_string().ends_with(".crs") {
                    path_files.push(new_path.path().display().to_string());
                }
            }
        }

        path_files
    }

    pub fn parse_file(path: String) -> String {
        fs::read_to_string(path).expect("Ошибка при чтении файла")
    }
}