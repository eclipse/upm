%feature("director") UpmCallback;
%apply int {mraa::Edge}

%inline %{
    class UpmCallback {
        public:
            UpmCallback() {mraa_fptr = &mraa_callback;};
            virtual ~UpmCallback() {}

            static void mraa_callback(void* self) {
                ((UpmCallback *)self)->run();
            }

            void (*mraa_fptr)(void*);// = (void (*)(void*))&UpmCallback::mraa_callback;
            virtual void run() {printf("UpmCallback::run()\n");}
    };
%}

%extend upm::Button {
    public:
        void installISR(mraa::Edge level, UpmCallback *cb) {
            $self->installISR(level, cb->mraa_fptr, cb);
        }
};
