namespace rat {
    template<typename Tuple>
    Dialog::Dialog(Tuple&& tuple, const std::string& file) :
    Module(tuple) {
        _interface = getModule<GUI>().addInterface(file);
        TextAreaWidget* area = new TextAreaWidget;
        area->setString("sadasdasdasdasdasdsadasdasdasdasdasdsadasdasdasdasdasdsa
        dasdasdasdasdasdsadasdasdasdasdasdsadasdasdasdasdasd");
        
        
    }
}