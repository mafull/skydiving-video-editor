# Ideas


| Idea | Category | Description | Notes |
| :--: | :--: | -- | -- |
| Local Docker environment | `Development` | Docker configuration for containerised local development.<br />Avoids the needs to configure and install everything locally when developing the project. | *To consider: Docker can vary across M1 vs Intel Macs :(* |
| Clipping | `Feature` | Nothing fancy here to begin with - this just covers splitting a larger video file up into one or more smaller clips. | *To consider: creating new files vs storing markers as metadata for the original video.* |
| Automatic montage creation | `Feature` | Given a set of clips, combine them together into a single video. | *Consider clip length, tags, etc. to decide on the order of the clips for best viewing experience.* |
| Automatic montage x music synchronisation | `Feature` | Given a set of clips and some music, combine the clips into a montage that's synchronised with the music. | *To consider: automatic clip ordering; "margins" per clip that say how much can be trimmed.* |
| Cloud storage | `Feature` | Starting to consider this as a distributed project, adding the ability to view and manage remote video files. | *Local caching could be a fun (and powerful) feature of this.* |
| Computer vision | `Feature` | Generating a skeletal human body representation from a wind tunnel or sky video.<br />Later extensible for coaching purposes.| *Yeah... I know... this is a **massive** project in itself... but wouldn't it be cool!* |
